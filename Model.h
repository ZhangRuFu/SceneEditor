#pragma once
/*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������Mesh
*	�衡����������һ��GameEntity����ۡ����ӻ�
*
*/

#include <vector>
#include <map>
#include <GLM\glm.hpp>
#include "Mesh.h"
#include "CommonType.h"
#include "ComponentManager.h"

using std::vector;
using std::map;
using glm::mat4;

class MeshManager;
class SphereCollider;

/*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������Model
*	�衡������ģ����ĳ������
*
*/
//������������������

class Model : public Component
{
	friend MeshManager;

private:
	int m_modelID;			//��ʶModel��IDΪ����ģ����Manager�е�����ֵ

private:
	void SetModelID(int id) { m_modelID = id; }

public:
	virtual bool hasAnimation() = 0;
	virtual void AddTexture(string texturePath) = 0;
	virtual int GetMeshCount() const = 0;
	virtual const Mesh* GetMesh(int index) const = 0;

	int GetModelID(void) { return m_modelID; }

};

/*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������ModelArg
*	�衡����������ģ������Ĳ���
*
*/

class ModelArg : public ComponentArg
{
	enum ModelConfig { BASICMESH = 1, NORMALMESH = 2, PURECOLOR = 4, COLLIDERMODEL = 8  };

	string m_modelPath;
	BasicMesh m_basicMesh;
	vec3 m_color;
	int m_modelConfig;
	SphereCollider *m_collider;
	

public:
	ModelArg(GameEntity &entity, string modelPath) : m_modelPath(modelPath), m_modelConfig(0), ComponentArg(entity) { m_modelConfig = NORMALMESH; }
	ModelArg(GameEntity &entity, BasicMesh basicMesh) : m_basicMesh(basicMesh), m_modelConfig(0), ComponentArg(entity) { m_modelConfig = BASICMESH; }
	ModelArg(GameEntity &entity, BasicMesh basicMesh, vec3 color) :  m_basicMesh(basicMesh), m_color(color), m_modelConfig(0), ComponentArg(entity) { m_modelConfig = BASICMESH | PURECOLOR; }
	ModelArg(GameEntity &entity, SphereCollider*collider, vec3 color) : m_basicMesh(BasicMesh::SPHERE), m_collider(collider), m_color(color), m_modelConfig(0), ComponentArg(entity) { m_modelConfig = COLLIDERMODEL; }

	virtual Component* CreateComponent(void);
};

/*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������StaticModel
*	�衡��������̬ģ��
*
*/
class StaticModel : public Model
{
private:
	vector<StaticMesh*>	m_subMeshes;
	vector<Texture*> m_texutres;

public:
	StaticModel(vector<StaticMesh*> subMeshes, vector<Texture*> textures);

	void AddTexture(string texturePath);
	
	virtual bool hasAnimation() { return false; }
	virtual int GetComponentType(void) { return (int)ComponentType::Mesh::StaticMesh; }
	virtual int GetMeshCount() const { return m_subMeshes.size(); }
	virtual const StaticMesh* GetMesh(int index) const { return m_subMeshes[index]; }
};

/*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������SkeletonModel
*	�衡��������̬ģ�͵�������
*
*/
struct Key
{
	int m_key;
	float m_value;

public:
	Key(int key, float value) :m_key(key), m_value(value) {}
};

class BoneAnimation
{
private:
	vector<Key>* m_translation[3];			//Translate��X��Y��Z������Key
	vector<Key>* m_rotate[3];				//Rotate��X��Y��Z������Key
	vector<Key>* m_scale[3];				//Scale��X��Y��Z������Key
	vec3* m_prePostRotate;

public:
	enum FrameType { TX, TY, TZ, RX, RY, RZ, SX, SY, SZ };

public:
	BoneAnimation()
	{
		for (int i = 0; i < 3; i++)
		{
			m_translation[i] = nullptr;
			m_rotate[i] = nullptr;
			m_scale[i] = nullptr;
			m_prePostRotate = nullptr;
		}
	}
	int GetKeyCount(FrameType type)
	{
		switch (type)
		{
		case FrameType::TX:
		case FrameType::TY:
		case FrameType::TZ:
			m_translation[type]->size();
			break;
		case FrameType::RX:
		case FrameType::RY:
		case FrameType::RZ:
			m_rotate[type % 3]->size();
			break;
		case FrameType::SX:
		case FrameType::SY:
		case FrameType::SZ:
			m_scale[type % 3]->size();
			break;
		}
	}

	void AddFrame(vector<Key>* values, FrameType type)
	{
		switch (type)
		{
		case FrameType::TX:
		case FrameType::TY:
		case FrameType::TZ:
			m_translation[type] = values;
			break;
		case FrameType::RX:
		case FrameType::RY:
		case FrameType::RZ:
			m_rotate[type % 3] = values;
			break;
		case FrameType::SX:
		case FrameType::SY:
		case FrameType::SZ:
			m_scale[type % 3] = values;
			break;
		}
	}
	void AddFrame(vec3 *prePostRotate)
	{
		m_prePostRotate = prePostRotate;
	}

	mat4 GetTransform(float key);
	vec3 GetKeyValue(float key, vector<Key>* Keyvector[3]);
};

//������Ϣ
struct Bone
{
	string m_name;
	mat4 m_offsetMatrix;
	int m_childCount;
	Bone *m_child;
	BoneAnimation *m_animation;

	Bone(void) : m_child(nullptr), m_animation(nullptr) {}
};

//�ɹ�������ù�����������������
struct BoneIndex
{
	Bone *bone;
	int index;
};



class BoneManager
{
public:
	BoneManager(Bone *rootBone);
	const BoneIndex* GetBoneIndex(string boneName);
	void UpdateSkeleton(float key, vector<mat4> &transform);
	void UpdateSkeleton(Bone *bone, mat4 &parentTransform, float key, vector<mat4> &transform);
	bool isRootBone(Bone *bone) { return bone == m_rootBone; }
	int GetBoneCount(void) { return m_bonesMap.size(); }

private:
	Bone *m_rootBone;
	map<string, BoneIndex> m_bonesMap;		//������-����ӳ���
};


class SkeletonModel : public Model
{
	friend class FBXModelLoader;
private:
	vector<SkeletonMesh> m_meshes;			//SkeletonSubMesh
	vector<Texture*> m_texutres;			//Texture
	
	BoneManager *m_boneManager;				//Bone
	
	int m_keyCount;							//����֡����

public:
	class SkeletonModelException
	{
	private:
		string m_err;

	public:
		SkeletonModelException(string err) :m_err(err) {}
	};

public:
	SkeletonModel(void);

	virtual bool hasAnimation(void) { return true; }
	virtual void AddTexture(string texturePath) {};
	virtual int GetComponentType(void) { return (int)ComponentType::Mesh::SkeletonMesh; }
	virtual int GetMeshCount() const { return m_meshes.size(); }
	virtual const SkeletonMesh* GetMesh(int index) const { return &m_meshes[index]; };

	int GetAnimationKeyCount() { return m_keyCount; }
	BoneManager* GetBoneManager() { return m_boneManager; }
};
