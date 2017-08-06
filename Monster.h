#pragma once
#include "Entity.h"
#include "Model.h"
#include "AnimationModelDrawer.h"
#include "RenderSystem.h"
#include "MeshManager.h"
#include "AnimationController.h"

/*
*	ÒýÇæ°æ±¾£ºDragon Engine v0.1;
*	Àà¡¡¡¡Ãû£ºMonster
*	Ãè¡¡¡¡Êö£ºÓÎÏ·Âß¼­£º¹ÖÎïÀà
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