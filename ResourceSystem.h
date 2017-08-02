#pragma once
/*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������ResourceSysyem
*	�衡������������Ϸ�е�������Դ��������GameEntity��Դ��Mesh��Դ��Texture��Դ
*
*/

#include "MeshManager.h"
#include "TextureManager.h"
#include "CollideManager.h"
#include "Entity.h"
#include "Camera.h"
#include "Light.h"
#include <list>

using std::list;

class DragonEngine;
class AnimationController;

class ResourceSystem
{
private:
	static ResourceSystem *m_instance;

private:
	list<GameEntity*> m_entityList;
	list<GameSpirit*> m_spiritList;
	//===========================================Camera��LightҲӦ�÷���spiritList��==========================================
	list<Camera*> m_cameraList;
	list<Light*> m_lightList;
	list<AnimationController*> m_animationControllers;
	
	DragonEngine *m_engine;
	MeshManager *m_meshManager;
	CollideManager *m_collideManager;
	TextureManager *m_textureManager;
	

private:
	ResourceSystem(DragonEngine *engine);

public:
	static ResourceSystem* GetInstance(DragonEngine *engine);
	
	//=========================================����Ӧ��ֻ����һ��ע�ắ���������ڽ��������ж�=====================================
	static void Register(GameEntity* entity);
	static void Register(GameSpirit *spirit);
	static void Register(Camera *camera);
	static void Register(Light *light);
	static void Register(AnimationController *animationController);
	static Camera* GetMainCamera();
	static Light* GetLight();

	MeshManager* GetMeshManager(void) { return m_meshManager; }
	CollideManager *GetCollideManager(void) { return m_collideManager; }

	void ReadyRender(void);

	void Init();
	void Move();
};