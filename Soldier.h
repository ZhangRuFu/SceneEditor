#pragma once
#include "Entity.h"
#include "Mesh.h"
#include "ModelDrawer.h"
#include "RenderSystem.h"
#include "InputSystem.h"
#include "CommonType.h"

/*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������Soldier
*	�衡��������Ϸ�߼���ʿ����
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