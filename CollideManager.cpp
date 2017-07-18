#include "CollideManager.h"
#include "Model.h"
#include "Collider.h"
#include "ResourceSystem.h"
#include "ColliderModelDrawer.h"

CollideManager::CollideManager(ResourceSystem * rSystem)
{
	m_resourceSystem = rSystem;
}

CollideManager * CollideManager::GetInstance(ResourceSystem * rSystem)
{
	if (m_instance == nullptr)
		m_instance = new CollideManager(rSystem);
	return m_instance;
}

void CollideManager::CalcModelBoundingBox(Model * model)
{
	int modelID = model->GetModelID();
	if (m_modelBoundingMap.find(modelID) == m_modelBoundingMap.end())
	{
		Collider *collider = new SphereCollider(nullptr);
		collider->CalcModelBoundingBox(model);
		m_modelBoundingMap[modelID] = collider;
	}
}

void CollideManager::Register(GameEntity * entity)
{
	//=================================================暂时假设物体只有一个网格====================================================
	Model *model = (Model*)entity->GetComponent((int)ComponentType::ComponentClass::Mesh);
	if (model == nullptr)
		return;
	int id = model->GetModelID();
	if (m_modelBoundingMap.find(id) != m_modelBoundingMap.end())
	{
		SphereCollider *collider = new SphereCollider(*(SphereCollider*)m_modelBoundingMap[id]);
		collider->SetTransform(entity->GetTransform());
		entity->GetComponentManager()->AddComponent(collider);

		//可视化碰撞体
		ModelArg mArg(collider, vec3(0.0f, 0.0f, 0.6f));
		entity->AddComponent(mArg);
	}
}


CollideManager *CollideManager::m_instance = nullptr;