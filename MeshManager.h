#pragma once
/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：MeshManager
*	描　　述：Mesh管理器，对所有Model包括静态和动态进行加载和管理，并提供基本几何Mesh进行创建
			必须先由DragonEngine进行初始化
*
*/
#include <vector>
#include <string>
#include <map>
#include "CommonType.h"

using std::string;
using std::vector;
using std::map;

class Model;
class ResourceSystem;

class MeshManager
{
private:
	static MeshManager *m_instance;

	ResourceSystem *m_resourceSystem;

	vector<Model*> m_meshes;							//注册所有游戏中的Model（资源唯一)
	map<string, int> m_meshMap;							//Model路径-索引 映射表
	map<string, int> m_namedMeshMap;					//命名Model-索引 映射表

public:
	Model* _LoadModel(string meshPath);					//加载自定义的模型文件
	Model* _LoadModel(BasicMesh basicMesh);				//加载基本几何体
	Model* _GetModel(string modelName);					//获取模型数据
	
	static Model* LoadModel(string meshPath);
	static Model* LoadModel(string meshPath, string meshName);
	static Model* LoadModel(BasicMesh basicMesh);
	static Model* GetModel(string modelName);
	static MeshManager* GetInstance(ResourceSystem *rSystem);

private:
	MeshManager(ResourceSystem *rSystem);
	void CreateBasicMesh(void);
};