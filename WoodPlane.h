#pragma once

#include "Entity.h"
#include "Model.h"
#include "ModelDrawer.h"

class WoodPlane : public GameEntity
{
private:
	Transform *m_transform;
	Model *m_model;

public:
	WoodPlane(void)
	{
		m_transform = (Transform*)GetComponent(ComponentType::Transform);

		ModelArg modelArg(*this, BasicMesh::PLANE);
		m_model = dynamic_cast<Model*>(AddComponent(modelArg));
		m_model->AddTexture("E:\\GameDevelop\\Asset\\Texture\\wood2.jpg");
	}
};