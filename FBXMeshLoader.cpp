#include <glm\gtc\matrix_transform.hpp>
#include <GLM\gtx\euler_angles.hpp>
#include <stack>
#include "FBXMeshLoader.h"
#include "TextureManager.h"

using std::stack;
using glm::eulerAngleX;
using glm::eulerAngleY;
using glm::eulerAngleZ;
using glm::radians;

FBXModelLoader::FBXModelLoader(string path) : MeshLoader(path)
{
	if (m_meshPath.empty())
		throw FBXMeshLoaderException("模型目录无效");
	m_scene = nullptr;
	m_model = nullptr;
}

Model * FBXModelLoader::LoadModel()
{
	//加载场景文件
	FbxManager *fbxManager = FbxManager::Create();
	FbxIOSettings *ios = FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(ios);
	FbxImporter *fbxImporter = FbxImporter::Create(fbxManager, "");
	if (!fbxImporter->Initialize(m_meshPath.c_str(), -1, fbxManager->GetIOSettings()))
	{
		string err = "FBX导入器初始化出错，错误信息：";
		err += fbxImporter->GetStatus().GetErrorString();
		throw FBXMeshLoaderException(err);
	}
	FbxScene *scene = FbxScene::Create(fbxManager, "myScene");
	FbxGeometryConverter geomConverter(fbxManager);
	geomConverter.Triangulate(scene, true);

	fbxImporter->Import(scene);
	m_scene = scene;

	m_meshPath = m_meshPath.substr(0, m_meshPath.find_last_of('\\') + 1);

	m_model = new SkeletonModel();
	//加载模型元信息
	LoadModelInfo(m_scene->GetGlobalSettings());

	//加载骨骼
	LoadBone();

	//加载网格
	LoadMesh(m_scene->GetRootNode());

	//加载动画
	LoadBoneAnimation();

	m_scene->Destroy(true);
	m_scene = nullptr;
	
	fbxManager->Destroy();
	return m_model;
}

void FBXModelLoader::LoadModelInfo(const FbxGlobalSettings &pGlobalSettings)
{
	//FbxGetTimeModeName(pGlobalSettings->GetTimeMode());

	FbxTimeSpan timeSpan;
	pGlobalSettings.GetTimelineDefaultTimeSpan(timeSpan);
	m_model->m_keyCount = timeSpan.GetDuration().GetFrameCount();
}


void FBXModelLoader::LoadMesh(FbxNode * node)
{
	int meshCount = m_scene->GetSrcObjectCount<FbxMesh>();
	for (int i = 0; i < meshCount; i++)
	{
		FbxMesh *mesh = m_scene->GetSrcObject<FbxMesh>(i);
		m_model->m_meshes.push_back(SkeletonMesh());
		MeshTransform(mesh, m_model->m_meshes[i]);
	}
}

void FBXModelLoader::MeshTransform(FbxMesh * mesh, SkeletonMesh &skeletonMesh)
{
	vector<SkeletonVertex> &vertexes = skeletonMesh.m_vertices;
	int ctlPointCount = mesh->GetControlPointsCount();
	VertexWeight *vertexBoneInfo = new VertexWeight[ctlPointCount];

	//加载顶点权重信息
	GetVertexWeight(mesh, vertexBoneInfo);

	//获取Position,Color,Normal,UV信息
	int polygonCount = mesh->GetPolygonCount();
	FbxVector4 *crlPoints = mesh->GetControlPoints();

	int vertexSize = polygonCount * 3;
	vertexes.resize(vertexSize);
	for (int i = 0; i < polygonCount; i++)
		for (int j = 0; j < 3; j++)
		{
			int index = i * 3 + j;
			int ctlPointIndex = mesh->GetPolygonVertex(i, j);
			FbxVector4 &ctlPoint = crlPoints[ctlPointIndex];
			vertexes[index].m_position.x = ctlPoint[0];
			vertexes[index].m_position.y = ctlPoint[1];
			vertexes[index].m_position.z = ctlPoint[2];
			vertexes[index].m_weight = vertexBoneInfo[ctlPointIndex];
		}

	GetNormal(mesh, vertexes);
	GetUV(mesh, vertexes);
	delete[] vertexBoneInfo;

	//贴图信息
	LoadTexture(mesh, "texDiffuse", skeletonMesh);
}

void FBXModelLoader::GetNormal(FbxMesh * mesh, vector<SkeletonVertex> &vertexes)
{
	FbxGeometryElementNormal* normalElement = mesh->GetElementNormal();
	if (!normalElement)
		return;

	bool useIndex = normalElement->GetReferenceMode() == FbxGeometryElement::EReferenceMode::eIndexToDirect;
	bool byCtrlPoint = normalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint;
	int normalCount = normalElement->GetDirectArray().GetCount();
	int normalIndex = -1;

	int polygonCount = mesh->GetPolygonCount();

	FbxVector4 fbxNormal;
	for (int i = 0; i < polygonCount; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int crlPointIndex = mesh->GetPolygonVertex(i, j);
			int polyPointIndex = i * 3 + j;

			if (byCtrlPoint)
				normalIndex = useIndex ? normalElement->GetIndexArray().GetAt(crlPointIndex) : crlPointIndex;
			else
				normalIndex = useIndex ? normalElement->GetIndexArray().GetAt(polyPointIndex) : polyPointIndex;
			fbxNormal = normalElement->GetDirectArray().GetAt(normalIndex);

			vertexes[polyPointIndex].m_normal.x = fbxNormal[0];
			vertexes[polyPointIndex].m_normal.y = fbxNormal[1];
			vertexes[polyPointIndex].m_normal.z = fbxNormal[2];
		}
	}
}

vec3 FBXModelLoader::GetColor(FbxMesh * mesh, int crlPointIndex, int polyPointIndex)
{
	vec3 color;
	FbxColor fbxColor;

	if (!mesh->GetElementVertexColorCount())
		return color;

	FbxGeometryElementVertexColor* vertexColor = mesh->GetElementVertexColor();
	int colorCount = vertexColor->GetDirectArray().GetCount();
	int colorIndex = 0;
	bool useIndex = vertexColor->GetReferenceMode() == FbxGeometryElement::EReferenceMode::eIndexToDirect;

	if (vertexColor->GetMappingMode() == FbxGeometryElement::EMappingMode::eByControlPoint)
		colorIndex = useIndex ? vertexColor->GetIndexArray().GetAt(crlPointIndex) : colorIndex;
	else if (vertexColor->GetMappingMode() == FbxGeometryElement::EMappingMode::eByPolygonVertex)
		colorIndex = useIndex ? vertexColor->GetIndexArray().GetAt(polyPointIndex) : polyPointIndex;
	fbxColor = vertexColor->GetDirectArray().GetAt(colorIndex);

	color.r = fbxColor.mRed;
	color.g = fbxColor.mGreen;
	color.b = fbxColor.mBlue;
	return color;
}

void FBXModelLoader::GetUV(FbxMesh * mesh, vector<SkeletonVertex> &vertexes)
{
	FbxVector2 uvValue;

	if (!mesh->GetElementUVCount())
		return;
	int uvLayerCount = mesh->GetUVLayerCount();

	FbxGeometryElementUV *uv = mesh->GetElementUV(0);

	const bool useIndex = uv->GetReferenceMode() == FbxGeometryElement::EReferenceMode::eIndexToDirect;
	bool byCtrlPoint = uv->GetMappingMode() == FbxGeometryElement::eByControlPoint;
	const int indexCount = useIndex ? uv->GetIndexArray().GetCount() : 0;
	int uvIndex = -1;

	int polygonCount = mesh->GetPolygonCount();
	for (int i = 0; i < polygonCount; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int crlPointIndex = mesh->GetPolygonVertex(i, j);
			int polyPointIndex = i * 3 + j;

			if (byCtrlPoint)
				uvIndex = useIndex ? uv->GetIndexArray().GetAt(crlPointIndex) : crlPointIndex;
			else
				uvIndex = useIndex ? uv->GetIndexArray().GetAt(polyPointIndex) : polyPointIndex;

			uvValue = uv->GetDirectArray().GetAt(uvIndex);
			vertexes[polyPointIndex].m_texCoord.x = uvValue[0];
			vertexes[polyPointIndex].m_texCoord.y = -uvValue[1];
		}
	}
}

void FBXModelLoader::LoadTexture(FbxMesh *mesh, string type, SkeletonMesh &skeletonMesh)
{
	vector<Texture*> &modelTextures = m_model->m_texutres;
	FbxNode* pNode = mesh->GetNode();
	if (!pNode)
		return;

	int materialCount = pNode->GetMaterialCount();

	for (int materialIndex = 0; materialIndex < materialCount; materialIndex++)
	{
		FbxSurfaceMaterial* surfaceMaterial = pNode->GetMaterial(materialIndex);
		int textureLayerIndex;
		FbxProperty pProperty;
		for (textureLayerIndex = 0; textureLayerIndex < fbxsdk::FbxLayerElement::sTypeTextureCount; ++textureLayerIndex)
		{
			pProperty = surfaceMaterial->FindProperty(fbxsdk::FbxLayerElement::sTextureChannelNames[textureLayerIndex]);
			if (pProperty.IsValid())
			{
				int textureCount = pProperty.GetSrcObjectCount<FbxTexture>();
				for (int j = 0; j < textureCount; ++j)
				{
					FbxFileTexture *pFileTexture = FbxCast<FbxFileTexture>(pProperty.GetSrcObject<FbxTexture>(j));
					bool isFind = false;
					Texture *tex = nullptr;
					if (pFileTexture)
					{
						string filePath(pFileTexture->GetFileName());
						filePath.replace(filePath.find_last_of('/'), 1, "\\");
						//防止重复加载
						for (int i = 0; i < modelTextures.size(); i++)
							if (modelTextures[i]->m_path == filePath)
							{
								tex = modelTextures[i];
								isFind = true;
								break;
							}
						if (!isFind)
						{
							//新贴图
							tex = new Texture();
							modelTextures.push_back(tex);
							tex->m_id = TextureManager::LoadTexture(filePath, false);
							tex->m_path = filePath;
							tex->m_type = type;
						}
						skeletonMesh.m_textures.push_back(tex);
					}
				}
			}
		}
	}
}

void FBXModelLoader::GetVertexWeight(FbxGeometry* pGeometry, VertexWeight *vertexBoneInfo)
{
	int skinCount = 0;
	int clusterCount = 0;
	FbxCluster* cluster = nullptr;
	//geometry矩阵
	FbxAMatrix geometryTransform = GetGeometryTransformation(pGeometry->GetNode());
	//获取【皮肤变形器】数量
	skinCount = pGeometry->GetDeformerCount(FbxDeformer::eSkin);

	for (int i = 0; i < skinCount; ++i)
	{
		//获取【骨骼】数量,这里的cluster就是骨骼
		clusterCount = ((FbxSkin *)pGeometry->GetDeformer(i, FbxDeformer::eSkin))->GetClusterCount();
		for (int j = 0; j < clusterCount; ++j)
		{
			cluster = ((FbxSkin *)pGeometry->GetDeformer(i, FbxDeformer::eSkin))->GetCluster(j);

			const BoneIndex *boneIndex = m_model->m_boneManager->GetBoneIndex(cluster->GetLink()->GetName());

			int ctlPointCount = cluster->GetControlPointIndicesCount();
			int* ctlPointIndex = cluster->GetControlPointIndices();
			double* weights = cluster->GetControlPointWeights();

			for (int k = 0; k < ctlPointCount; k++)
				vertexBoneInfo[ctlPointIndex[k]].AddVertexBoneInfo(boneIndex->index, (float)weights[k]);

			FbxAMatrix transformMatrix;
			FbxAMatrix transformLinkMatrix;				//骨骼空间->世界空间
			FbxAMatrix globalBindposeInverseMatrix;

			cluster->GetTransformMatrix(transformMatrix);
			cluster->GetTransformLinkMatrix(transformLinkMatrix);
			globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix * geometryTransform;

			//获得偏移矩阵
			FbxMatrixToGLM(globalBindposeInverseMatrix, boneIndex->bone->m_offsetMatrix);
		}
	}
}


//骨骼信息加载
void FBXModelLoader::LoadBone(void)
{
	//先找到根节点
	Bone *rootBone = nullptr;
	FbxNode *root = m_scene->GetRootNode();
	FbxNode *rootBoneNode = nullptr;
	for (int i = 0; i < root->GetChildCount(); i++)
	{
		FbxNode *node = root->GetChild(i);
		int childCount = node->GetChildCount();
		if (childCount)
		{
			for (int j = 0; j < childCount; j++)
			{
				FbxNode *SkeletonNode = node->GetChild(j);
				FbxNodeAttribute *attrib = SkeletonNode->GetNodeAttribute();
				if (attrib && attrib->GetAttributeType() == FbxNodeAttribute::EType::eSkeleton)
				{
					rootBoneNode = node;
					rootBone = new Bone();
					goto finded;
				}
			}
		}
	}
finded:
	if (rootBone == nullptr)
		throw FBXMeshLoaderException("未找到根骨骼节点");

	//找到根骨骼
	LoadBone(rootBoneNode, rootBone);
	//骨骼加载完毕
	m_model->m_boneManager = new BoneManager(rootBone);
}

void FBXModelLoader::LoadBone(FbxNode * node, Bone *bone)
{
	string nodeName = node->GetName();
	bone->m_name = nodeName;
	bone->m_childCount = node->GetChildCount();
	bone->m_child = new Bone[bone->m_childCount];

	for (int i = 0; i < node->GetChildCount(); i++)
		LoadBone(node->GetChild(i), bone->m_child + i);
}



void FBXModelLoader::LoadBoneAnimation(void)
{
	FbxNode *rootNode = m_scene->GetRootNode();
	for (int i = 0; i < m_scene->GetSrcObjectCount<FbxAnimStack>(); i++)
	{
		FbxAnimStack* animStack = m_scene->GetSrcObject<FbxAnimStack>(i);
		int animLayerCount = animStack->GetMemberCount<FbxAnimLayer>();

		for (int j = 0; j < animLayerCount; j++)
		{
			FbxAnimLayer* animLayer = animStack->GetMember<FbxAnimLayer>(j);
			LoadAnimationLayerInfo(animLayer, rootNode);
		}
	}
}

void FBXModelLoader::LoadAnimationLayerInfo(FbxAnimLayer* pAnimLayer, FbxNode* pNode)
{
	string boneName = pNode->GetName();

	if (m_model->m_boneManager->GetBoneIndex(boneName) != nullptr)
		LoadChannels(pNode, pAnimLayer);

	for (int i = 0; i < pNode->GetChildCount(); i++)
		LoadAnimationLayerInfo(pAnimLayer, pNode->GetChild(i));
}

void FBXModelLoader::LoadChannels(FbxNode* node, FbxAnimLayer* animationLayer)
{
	Bone *bone = m_model->m_boneManager->GetBoneIndex(node->GetName())->bone;
	bone->m_animation = new BoneAnimation();
	BoneAnimation &boneAnimation = *bone->m_animation;

	LoadTranslate(node, animationLayer, boneAnimation);
	LoadRotate(node, animationLayer, boneAnimation);
	LoadScale(node, animationLayer, boneAnimation);
	if (m_model->m_boneManager->isRootBone(bone))
	{
		//根骨骼需要加载额外的信息以纠正整个模型的位置
		vec3 *prePost = new vec3[2];
		FbxDouble  *fbxDouble = node->PreRotation.Get().Buffer();
		prePost[0].x = fbxDouble[0];
		prePost[0].y = fbxDouble[1];
		prePost[0].z = fbxDouble[2];
		fbxDouble = node->PostRotation.Get().Buffer();
		prePost[1].x = fbxDouble[0];
		prePost[1].y = fbxDouble[1];
		prePost[1].z = fbxDouble[2];
		boneAnimation.AddFrame(prePost);
	}
}

void FBXModelLoader::LoadTranslate(FbxNode *node, FbxAnimLayer* animLayer, BoneAnimation &bone)
{
	FbxAnimCurve *curve = nullptr;
	vector<Key> *t[3]{ nullptr,nullptr,nullptr };
	FbxDouble3 v3 = node->LclTranslation.Get();
	const char* fbxCurveComponet[] = { FBXSDK_CURVENODE_COMPONENT_X, FBXSDK_CURVENODE_COMPONENT_Y, FBXSDK_CURVENODE_COMPONENT_Z };
	for (int i = 0; i < 3; i++)
	{
		curve = node->LclTranslation.GetCurve(animLayer, fbxCurveComponet[i]);
		if (curve)
			t[i] = LoadCurveKeys(curve);
		else
		{
			t[i] = new vector<Key>();
			t[i]->push_back(Key(0, v3.mData[i]));
		}
		bone.AddFrame(t[i], (BoneAnimation::FrameType)(BoneAnimation::FrameType::TX + i));
	}

	/*curve = node->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
	if (curve)
	t[0] = LoadCurveKeys(curve);
	else
	{
	t[0] = new vector<Key>();
	t[0]->push_back(Key(0, v3.mData[0]));
	}
	curve = node->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
	if (curve)
	t[1] = LoadCurveKeys(curve);
	else
	{
	t[1] = new vector<Key>();
	t[1]->push_back(Key(0, v3.mData[1]));
	}
	curve = node->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
	if (curve)
	t[2] = LoadCurveKeys(curve);
	else
	{
	t[2] = new vector<Key>();
	t[2]->push_back(Key(0, v3.mData[2]));
	}
	for (int i = 0; i < 3; i++)
	bone.AddFrame(t[i], (BoneAnimation::FrameType)(BoneAnimation::FrameType::TX + i));*/
}
void FBXModelLoader::LoadRotate(FbxNode *node, FbxAnimLayer* animLayer, BoneAnimation &bone)
{
	FbxAnimCurve *curve = nullptr;
	vector<Key> *r[3]{ nullptr,nullptr,nullptr };
	FbxDouble3 v3 = node->LclRotation.Get();
	const char* fbxCurveComponet[] = { FBXSDK_CURVENODE_COMPONENT_X, FBXSDK_CURVENODE_COMPONENT_Y, FBXSDK_CURVENODE_COMPONENT_Z };
	for (int i = 0; i < 3; i++)
	{
		curve = node->LclRotation.GetCurve(animLayer, fbxCurveComponet[i]);
		if (curve)
			r[i] = LoadCurveKeys(curve);
		else
		{
			r[i] = new vector<Key>();
			r[i]->push_back(Key(0, v3.mData[i]));
		}
		bone.AddFrame(r[i], (BoneAnimation::FrameType)(BoneAnimation::FrameType::RX + i));
	}

	/*curve = node->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
	if (curve)
	r[0] = LoadCurveKeys(curve);
	else
	{
	r[0] = new vector<Key>();
	r[0]->push_back(Key(0, v3.mData[0]));
	}
	curve = node->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
	if (curve)
	r[1] = LoadCurveKeys(curve);
	else
	{
	r[1] = new vector<Key>();
	r[1]->push_back(Key(0, v3.mData[1]));
	}
	curve = node->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
	if (curve)
	r[2] = LoadCurveKeys(curve);
	else
	{
	r[2] = new vector<Key>();
	r[2]->push_back(Key(0, v3.mData[2]));
	}
	for (int i = 0; i < 3; i++)
	bone.AddFrame(r[i], (BoneAnimation::FrameType)(BoneAnimation::FrameType::RX + i));*/
}
void FBXModelLoader::LoadScale(FbxNode *node, FbxAnimLayer* animLayer, BoneAnimation &bone)
{
	FbxAnimCurve *curve = nullptr;
	vector<Key> *s[3]{ nullptr,nullptr,nullptr };
	FbxDouble3 v3 = node->LclScaling.Get();
	curve = node->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
	const char* fbxCurveComponet[] = { FBXSDK_CURVENODE_COMPONENT_X, FBXSDK_CURVENODE_COMPONENT_Y, FBXSDK_CURVENODE_COMPONENT_Z };
	for (int i = 0; i < 3; i++)
	{
		curve = node->LclScaling.GetCurve(animLayer, fbxCurveComponet[i]);
		if (curve)
			s[i] = LoadCurveKeys(curve);
		else
		{
			s[i] = new vector<Key>();
			s[i]->push_back(Key(0, v3.mData[i]));
		}
		bone.AddFrame(s[i], (BoneAnimation::FrameType)(BoneAnimation::FrameType::SX + i));
	}
	/*if (curve)
	s[0] = LoadCurveKeys(curve);
	else
	{
	s[0] = new vector<Key>();
	s[0]->push_back(Key(0, v3.mData[0]));
	}
	curve = node->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
	if (curve)
	s[1] = LoadCurveKeys(curve);
	else
	{
	s[1] = new vector<Key>();
	s[1]->push_back(Key(0, v3.mData[1]));
	}
	curve = node->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
	if (curve)
	s[2] = LoadCurveKeys(curve);
	else
	{
	s[2] = new vector<Key>();
	s[2]->push_back(Key(0, v3.mData[2]));
	}
	for (int i = 0; i < 3; i++)
	bone.AddFrame(s[i], (BoneAnimation::FrameType)(BoneAnimation::FrameType::SX + i));*/
}
vector<Key>* FBXModelLoader::LoadCurveKeys(FbxAnimCurve* pCurve)
{
	vector<Key>* values = new vector<Key>();
	//关键帧数量
	int keyCount = pCurve->KeyGetCount();
	values->reserve(keyCount + 1);
	for (int i = 0; i < keyCount; i++)
	{
		int frame = pCurve->KeyGetTime(i).GetFrameCount();
		float value = pCurve->KeyGetValue(i);
		values->push_back(Key(frame, value));
	}
	return values;
}

void FBXModelLoader::FbxMatrixToGLM(FbxAMatrix & fbxMatrix, mat4 & glmMatrix)
{
	for (int i = 0; i < 4;i++)
		for (int j = 0; j < 4; j++)
			glmMatrix[i][j] = fbxMatrix[i][j];
}

FbxAMatrix FBXModelLoader::GLMToFbxMatrix(mat4 & glmMatrix)
{
	FbxAMatrix matrix;
	for (int i = 0; i < 4;i++)
		for (int j = 0; j < 4; j++)
			matrix[i][j] = glmMatrix[i][j];
	return matrix;
}

FbxAMatrix FBXModelLoader::GetGeometryTransformation(FbxNode* inNode)
{
	if (!inNode)
	{
		throw std::exception("Null for mesh geometry");
	}

	const FbxVector4 lT = inNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 lR = inNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 lS = inNode->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(lT, lR, lS);
}



mat4 BoneAnimation::GetTransform(float key)
{
	//从帧集合找到指定帧相邻2个帧进行线性插值
	vec3 trans = GetKeyValue(key, m_translation);
	vec3 rotat = GetKeyValue(key, m_rotate);
	vec3 scal = GetKeyValue(key, m_scale);
	mat4 transform;
	mat4 rotateM = (mat4)(eulerAngleZ(radians((double)rotat.z)) * eulerAngleY(radians((double)rotat.y)) * eulerAngleX(radians((double)rotat.x)));
	if (m_prePostRotate != nullptr)
	{
		mat4 preRotate = (mat4)(eulerAngleZ(radians((double)m_prePostRotate[0].z)) * eulerAngleY(radians((double)m_prePostRotate[0].y)) * eulerAngleX(radians((double)m_prePostRotate[0].x)));
		mat4 postRotate = (mat4)(eulerAngleZ(radians((double)m_prePostRotate[1].z)) * eulerAngleY(radians((double)m_prePostRotate[1].y)) * eulerAngleX(radians((double)m_prePostRotate[1].x)));
		rotateM = preRotate * rotateM * postRotate;
	}
	transform = translate(transform, trans);
	transform *= rotateM;
	transform = scale(transform, scal);

	return transform;
}

vec3 BoneAnimation::GetKeyValue(float key, vector<Key>* Keyvector[3])
{
	vec3 keyValue;
	int pre = 0, next = 0;
	for (int i = 0; i < 3; i++)
	{
		int j;
		for (j = 0; j < Keyvector[i]->size(); j++)
		{
			if (key <= (*Keyvector[i])[j].m_key)
			{
				pre = j - 1 >= 0 ? j - 1 : j;
				next = j;
				break;
			}
		}
		if (j == Keyvector[i]->size())
			pre = next = j - 1;
		double ratio = pre == next ? 0 : (double)(key - (*Keyvector[i])[pre].m_key) / ((*Keyvector[i])[next].m_key - (*Keyvector[i])[pre].m_key);
		keyValue[i] = (*Keyvector[i])[pre].m_value * (1 - ratio) + (*Keyvector[i])[next].m_value * ratio;
	}
	return keyValue;
}