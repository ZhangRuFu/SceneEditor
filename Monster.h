#pragma once
#include "Entity.h"
#include "Model.h"
#include "AnimationModelDrawer.h"
#include "RenderSystem.h"
#include "MeshManager.h"
#include "AnimationController.h"

/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：Monster
*	描　　述：游戏逻辑：怪物类
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