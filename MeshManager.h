#pragma once
/*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������MeshManager
*	�衡������Mesh��������������Model������̬�Ͷ�̬���м��غ͹������ṩ��������Mesh���д���
			��������DragonEngine���г�ʼ��
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

	vector<Model*> m_meshes;							//ע��������Ϸ�е�Model����ԴΨһ)
	map<string, int> m_meshMap;							//Model·��-���� ӳ���
	map<string, int> m_namedMeshMap;					//����Model-���� ӳ���

public:
	Model* _LoadModel(string meshPath);					//�����Զ����ģ���ļ�
	Model* _LoadModel(BasicMesh basicMesh);				//���ػ���������
	Model* _GetModel(string modelName);					//��ȡģ������
	
	static Model* LoadModel(string meshPath);
	static Model* LoadModel(string meshPath, string meshName);
	static Model* LoadModel(BasicMesh basicMesh);
	static Model* GetModel(string modelName);
	static MeshManager* GetInstance(ResourceSystem *rSystem);

private:
	MeshManager(ResourceSystem *rSystem);
	void CreateBasicMesh(void);
};