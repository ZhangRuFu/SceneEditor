#pragma once
#include "Entity.h"
#include "Mesh.h"
#include "ModelDrawer.h"
#include "RenderSystem.h"

/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：Soldier
*	描　　述：游戏逻辑：士兵类
*
*/
class Soldier : public GameSpirit
{
public:
	Soldier(void)
	{
		ModelArg mArg("E:\\GameDevelop\\Model\\nanosuit\\nanosuit.obj");
		AddComponent(mArg);
	}

	void Move()
	{

	}
};