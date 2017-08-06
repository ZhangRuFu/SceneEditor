#include "Model.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "Time.h"
#include "ModelDrawer.h"
#include "AnimationModelDrawer.h"
#include "SimpleModelDrawer.h"
#include "ColliderModelDrawer.h"
#include "AnimationController.h"
#include "Collider.h"
#include <stack>

using std::stack;

StaticModel::StaticModel(vector<StaticMesh*> subMeshes, vector<Texture*> textures) : m_subMeshes(subMeshes), m_texutres(textures) {}

void StaticModel::AddTexture(string texturePath)
{
	unsigned int texID = TextureManager::LoadTexture(texturePath, false);
	Texture *tex = new Texture();
	tex->m_id = texID;
	tex->m_path = texturePath;
	tex->m_type = "texDiffuse";
	m_subMeshes[0]->AddTexture(tex);
	m_texutres.push_back(tex);
}



//==================================================SkeletonMesh================================================
SkeletonModel::SkeletonModel(void)
{
	m_boneManager = nullptr;
}

BoneManager::BoneManager(Bone * rootBone)
{
	m_rootBone = rootBone;
	//遍历骨骼，建立映射关系
	stack<Bone*> boneStack;

	//利用栈进行非递归的树遍历
	boneStack.push(m_rootBone);
	while (!boneStack.empty())
	{
		Bone *curBone = boneStack.top();
		boneStack.pop();
		BoneIndex *boneIndex = new BoneIndex();
		boneIndex->bone = curBone;
		boneIndex->index = m_bonesMap.size();
		m_bonesMap[curBone->m_name] = *boneIndex;
		for (int i = 0; i < curBone->m_childCount; i++)
			boneStack.push(curBone->m_child + i);
	}
}

const BoneIndex* BoneManager::GetBoneIndex(string boneName)
{
	if (m_bonesMap.find(boneName) != m_bonesMap.end())
		return &m_bonesMap[boneName];
	return nullptr;
}

void BoneManager::UpdateSkeleton(float key, vector<mat4> &transform)
{
	transform.resize(m_bonesMap.size());
	UpdateSkeleton(m_rootBone, mat4(), key, transform);
}

void BoneManager::UpdateSkeleton(Bone *bone, mat4 &parentTransform, float key, vector<mat4> &transform)
{
	string boneName = bone->m_name;
	mat4 localBoneTrans;
	if (m_bonesMap.find(boneName) != m_bonesMap.end())
	{
		const BoneIndex &boneIndex = m_bonesMap[boneName];
		BoneAnimation &boneAnimation = *boneIndex.bone->m_animation;
		localBoneTrans = boneAnimation.GetTransform(key);
		transform[boneIndex.index] = parentTransform * localBoneTrans * boneIndex.bone->m_offsetMatrix;
	}
	for (int i = 0; i < bone->m_childCount; i++)
		UpdateSkeleton(bone->m_child + i, parentTransform * localBoneTrans, key, transform);
}

Component * ModelArg::CreateComponent(void)
{
	Model *modelCom = nullptr;
	Transform *transform = dynamic_cast<Transform*>(GetComponent(ComponentType::Transform));
	if (m_modelConfig & ModelConfig::NORMALMESH)
	{
		 //静态、骨骼模型文件
		 modelCom = dynamic_cast<Model*>(MeshManager::LoadModel(m_modelPath));
		 int type = modelCom->GetComponentType();
		 if (type == (int)ComponentType::Mesh::StaticMesh)
			 AddComponent(StaticModelDrawer::Create(modelCom));
		 else if (type == (int)ComponentType::Mesh::SkeletonMesh)
		 {
			 AnimationModelDrawer*drawer = AnimationModelDrawer::Create(modelCom);
			 AddComponent(drawer);
			 AddComponent(new AnimationController(drawer, (SkeletonModel*)modelCom));
		 }
	}
	else if(m_modelConfig & ModelConfig::BASICMESH)
	{
		//基本内置模型
		modelCom = MeshManager::LoadModel(m_basicMesh);
		if (m_modelConfig & ModelConfig::PURECOLOR)
			AddComponent(SimpleModelDrawer::Create(modelCom, m_color));
		else
			AddComponent(StaticModelDrawer::Create(modelCom));
	}
	else
	{
		modelCom = MeshManager::LoadModel(m_basicMesh);
		AddComponent(ColliderModelDrawer::Create(modelCom, m_collider, m_color));
	}
	return modelCom;
}
