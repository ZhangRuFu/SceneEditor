#pragma once
/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：FBXMeshLoader
*	描　　述：FBX模型文件的模型加载器
*
*/

#include "MeshLoader.h"
#include "Mesh.h"
#include <fbxsdk.h>

class FBXModelLoader : public MeshLoader
{
private:
	FbxScene *m_scene;						//FBX场景指针
	SkeletonModel *m_model;					//动态Mesh


public:
	FBXModelLoader(string path);

	//Override
	virtual Model* LoadModel();


public:
	class FBXMeshLoaderException
	{
	private:
		string m_err;

	public:
		FBXMeshLoaderException(string err) :m_err(err) {}
	};

private:
	//模型信息
	void LoadModelInfo(const FbxGlobalSettings &pGlobalSettings);

	//网格信息
	void LoadMesh(FbxNode *node);

	//骨骼信息
	void LoadBone(void);
	void LoadBone(FbxNode *node, Bone *bone);

	//网格信息转换
	void MeshTransform(FbxMesh * mesh, SkeletonMesh &skeletonMesh);
	void GetVertexWeight(FbxGeometry* pGeometry, VertexWeight *vertexBoneInfo);		//顶点骨骼信息（骨骼索引，权重信息）
	void GetNormal(FbxMesh *mesh, vector<SkeletonVertex> &vertexes);				//法线信息
	vec3 GetColor(FbxMesh *mesh, int crlPointIndex, int polyPointIndex);			//颜色信息
	void GetUV(FbxMesh *mesh, vector<SkeletonVertex> &vertexes);					//UV坐标信息
	void LoadTexture(FbxMesh *mesh, string type, SkeletonMesh &skeletonMesh);		//加载贴图

	//动画信息
	void LoadBoneAnimation(void);
	void LoadAnimationLayerInfo(FbxAnimLayer* pAnimLayer, FbxNode* pNode);
	void LoadChannels(FbxNode* pNode, FbxAnimLayer* pAnimLayer);
	void LoadTranslate(FbxNode *node, FbxAnimLayer* animLayer, BoneAnimation &bone);
	void LoadRotate(FbxNode *node, FbxAnimLayer* animLayer, BoneAnimation &bone);
	void LoadScale(FbxNode *node, FbxAnimLayer* animLayer, BoneAnimation &bone);
	FbxAMatrix GetGeometryTransformation(FbxNode* inNode);
	vector<Key>*  LoadCurveKeys(FbxAnimCurve* pCurve);

public:
	//转换工具
	static void FbxMatrixToGLM(FbxAMatrix& fbxMatrix, mat4& glmMatrix);
	static FbxAMatrix GLMToFbxMatrix(mat4& glmMatrix);
};