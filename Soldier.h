#pragma once
#include "Entity.h"
#include "Mesh.h"
#include "ModelDrawer.h"
#include "RenderSystem.h"
#include "InputSystem.h"
#include "CommonType.h"

/*
*	ÒýÇæ°æ±¾£ºDragon Engine v0.1;
*	Àà¡¡¡¡Ãû£ºSoldier
*	Ãè¡¡¡¡Êö£ºÓÎÏ·Âß¼­£ºÊ¿±øÀà
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