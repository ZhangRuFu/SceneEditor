#pragma once
#include "Entity.h"
#include "Model.h"
#include "AnimationModelDrawer.h"
#include "RenderSystem.h"
#include "MeshManager.h"
#include "AnimationController.h"

/*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������Monster
*	�衡��������Ϸ�߼���������
*
*/
class Monster : public GameSpirit
{
private:
	SkeletonModel *m_model;

public:
	Monster(void)
	{
		ModelArg mArg(*this, "E:\\GameDevelop\\Model\\Monster_Animation\\monster.FBX");
		m_model = dynamic_cast<SkeletonModel*>(AddComponent(mArg));
	}

	void Move()
	{

	}
};