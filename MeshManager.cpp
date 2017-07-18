#include "MeshManager.h"
#include "MeshLoaderFactory.h"
#include "Model.h"
#include "DragonEngine.h"
#include "ResourceSystem.h"

MeshManager::MeshManager(ResourceSystem *rSystem)
{
	m_resourceSystem = rSystem;
	CreateBasicMesh();
}

Model * MeshManager::_LoadModel(string meshPath)
{
	//防止重复加载
	if (m_meshMap.find(meshPath) != m_meshMap.end())
		return m_meshes[m_meshMap[meshPath]];

	//需要从硬盘加载模型文件
	MeshLoader *meshLoader = MeshLoaderFactory::GetMeshLoader(meshPath);
	//======================================Loader唯一可以避免重复加载、释放===================================
	Model *model = meshLoader->LoadModel();
	delete meshLoader;
	int modelID = m_meshes.size();
	m_meshMap[meshPath] = modelID;
	m_meshes.push_back(model);
	model->SetModelID(modelID);
	m_resourceSystem->GetCollideManager()->CalcModelBoundingBox(model);
	return model;
}

Model * MeshManager::_LoadModel(BasicMesh basicMesh)
{
	Model *model = new StaticModel(*dynamic_cast<StaticModel*>(m_meshes[basicMesh]));
	return model;
}

Model * MeshManager::_GetModel(string modelName)
{
	if (m_namedMeshMap.find(modelName) != m_namedMeshMap.end())
		return m_meshes[m_namedMeshMap[modelName]];
}

Model * MeshManager::LoadModel(string meshPath)
{
	return m_instance->_LoadModel(meshPath);
}

Model * MeshManager::LoadModel(string meshPath, string meshName)
{
	Model *model = m_instance->_LoadModel(meshPath);
	if (model != nullptr)
		m_instance->m_namedMeshMap[meshName] = m_instance->m_meshMap[meshPath];
	return model;
}

Model * MeshManager::LoadModel(BasicMesh basicMesh)
{
	return m_instance->_LoadModel(basicMesh);
}

Model * MeshManager::GetModel(string modelName)
{
	return m_instance->_GetModel(modelName);
}

MeshManager * MeshManager::GetInstance(ResourceSystem *rSystem)
{
	if (m_instance == nullptr)
		m_instance = new MeshManager(rSystem);
	return m_instance;
}

void MeshManager::CreateBasicMesh(void)
{
	m_meshes.resize(BasicMesh::BASICMESHMAX);
	CollideManager *collideManager = m_resourceSystem->GetCollideManager();

	//长方体
	float cubeVertices[] = {
		// Positions          // Normals           // Texture Coords
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,

		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,

		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

		1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,

		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f
	};
	int rectFaceIndex[] = { 0, 1, 2, 2, 3, 0 };
	int cubeVertexGroupCount = sizeof(cubeVertices) / (8 * 4);
	vector<Vertex> *cubeVertex = new vector<Vertex>(cubeVertexGroupCount);
	for (int i = 0; i < cubeVertexGroupCount; i++)
	{
		Vertex vertex;
		for (int j = 0; j < 3; j++)
			vertex.m_position[j] = cubeVertices[i * 8 + j];
		for (int j = 0; j < 3; j++)
			vertex.m_normal[j] = cubeVertices[i * 8 + 3 + j];
		for (int j = 0; j < 2; j++)
			vertex.m_texCoord[j] = cubeVertices[i * 8 + 6 + j];
		(*cubeVertex)[i] = (vertex);
	}
	vector<unsigned int> cubeIndex;
	for (int i = 0; i < 6; i++)
		for (int j = 0;j < 6; j++)
			cubeIndex.push_back(i * 4 + rectFaceIndex[j]);
	StaticMesh *cubeSubMesh = new StaticMesh(cubeVertex, &vector<Texture*>(), &cubeIndex);
	Model *cubeMesh = new StaticModel(vector<StaticMesh*>(1, cubeSubMesh), vector<Texture*>());
	cubeMesh->SetModelID(BasicMesh::CUBE);
	m_meshes[BasicMesh::CUBE] = cubeMesh;
	collideManager->CalcModelBoundingBox(cubeMesh);

	//平面
	float planeVertices[] = {
		// Positions			//Normal			// Texture Coords
		0.5f,  0.0f, 0.5f,		0.0f, 1.0f,	0.0f,	5.0f, 0.0f,
		-0.5f, 0.0f, 0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 5.0f,
		0.5f, 0.0f, -0.5f,		0.0f, 1.0f, 0.0f,	5.0f, 5.0f,
	};
	int planeVertexGroupCount = sizeof(planeVertices) / (8 * 4);
	vector<Vertex> *planeVertex = new vector<Vertex>(planeVertexGroupCount);
	for (int i = 0; i < planeVertexGroupCount; i++)
	{
		Vertex vertex;
		for (int j = 0; j < 3; j++)
			vertex.m_position[j] = planeVertices[i * 8 + j];
		for (int j = 0; j < 3; j++)
			vertex.m_normal[j] = planeVertices[i * 8 + 3 + j];
		for (int j = 0; j < 2; j++)
			vertex.m_texCoord[j] = planeVertices[i * 8 + 6 + j];
		(*planeVertex)[i] = (vertex);
	}
	vector<unsigned int> planeIndex;
	for (int i = 0; i < 6; i++)
		planeIndex.push_back(rectFaceIndex[i]);
	StaticMesh *planeSubMesh = new StaticMesh(planeVertex, &vector<Texture*>(), &planeIndex);
	Model *planeMesh = new StaticModel(vector<StaticMesh*>(1, planeSubMesh), vector<Texture*>());
	planeMesh->SetModelID(BasicMesh::PLANE);
	m_meshes[BasicMesh::PLANE] = planeMesh;
	collideManager->CalcModelBoundingBox(planeMesh);

	//球
	float thetaStep = 10;
	float betaStep = 18;
	int betaCount = 360 / betaStep;
	int thetaCount = 180 / thetaStep + 1;
	int sphereVertexCount = thetaCount * betaCount;
	thetaStep = radians(thetaStep);
	betaStep = radians(betaStep);
	vector<Vertex> *sphereVertex = new vector<Vertex>(sphereVertexCount);

	int sIndex = 0;
	for (float theta = 0; theta <= radians(180.0); theta += thetaStep)
	{
		float y = cos(theta);
		float r = sin(theta);
		for (float beta = 0; beta < radians(360.0); beta += betaStep)
		{
			(*sphereVertex)[sIndex].m_position.x = r * cos(beta);
			(*sphereVertex)[sIndex].m_position.y = y;
			(*sphereVertex)[sIndex].m_position.z = r * sin(beta);
			(*sphereVertex)[sIndex].m_normal = (*sphereVertex)[sIndex].m_position;
			(*sphereVertex)[sIndex].m_texCoord.r = beta / (radians(360.0));
			(*sphereVertex)[sIndex].m_texCoord.s = 1 - theta / radians(180.0);
			sIndex++;
		}
	}

	vector<unsigned int> sphereIndex;
	
	for (int i = 0; i < betaCount - 1; i++)
	{
		sphereIndex.push_back(i);
		sphereIndex.push_back(betaCount + i);
		sphereIndex.push_back(betaCount + i + 1);
	}
	sphereIndex.push_back(betaCount - 1);
	sphereIndex.push_back(2 * betaCount - 1);
	sphereIndex.push_back(2 * betaCount);

	int curIndex = 0;
	for (int i = 1; i < thetaCount - 2; i++)
	{
		for (int j = 0; j < betaCount - 1; j++)
		{
			curIndex = i * betaCount + j;
			sphereIndex.push_back(curIndex);
			sphereIndex.push_back(curIndex + betaCount);
			sphereIndex.push_back(curIndex + betaCount + 1);
			sphereIndex.push_back(curIndex + betaCount + 1);
			sphereIndex.push_back(curIndex + 1);
			sphereIndex.push_back(curIndex);
		}
		curIndex = (i + 1) * betaCount - 1;
		sphereIndex.push_back(curIndex);
		sphereIndex.push_back(curIndex + betaCount);
		sphereIndex.push_back(curIndex + 1);
		sphereIndex.push_back(curIndex + 1);
		sphereIndex.push_back(curIndex - betaCount + 1);
		sphereIndex.push_back(curIndex);
	}

	curIndex = (thetaCount - 1) * betaCount;
	for (int i = 0; i < betaCount - 1; i++)
	{
		sphereIndex.push_back(curIndex + i);
		sphereIndex.push_back(curIndex + i - betaCount);
		sphereIndex.push_back(curIndex + i - betaCount + 1);
	}
	sphereIndex.push_back(curIndex + betaCount - 1);
	sphereIndex.push_back(curIndex - 1);
	sphereIndex.push_back(curIndex - betaCount);
	StaticMesh *sphereSubMesh = new StaticMesh(sphereVertex, &vector<Texture*>(), &sphereIndex);
	Model *sphereMesh = new StaticModel(vector<StaticMesh*>(1, sphereSubMesh), vector<Texture*>());
	sphereMesh->SetModelID(BasicMesh::SPHERE);
	m_meshes[BasicMesh::SPHERE] = sphereMesh;
	collideManager->CalcModelBoundingBox(sphereMesh);
}

MeshManager * MeshManager::m_instance = nullptr;