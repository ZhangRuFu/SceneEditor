#pragma once
/*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������DragonEngine
*	�衡������Dragon��Ϸ����ʵ��
*
*/
#include "RenderSystem.h"
#include "ResourceSystem.h"
#include "InputSystem.h"
#include "WindowSystem.h"
#include "UI.h"
#include "Time.h"
#include "CommonType.h"

class DragonEngine
{
private:
	static DragonEngine *m_instance;

	GameState m_gameState;
	GameLoopState m_gameLoopState;
	RenderSystem *m_renderSystem;
	ResourceSystem *m_resourceSystem;
	InputSystem *m_inputSystem;
	WindowSystem *m_windowSystem;
	Time *m_time;

private:
	DragonEngine(void);

public:
	void Move();

	void Draw();

	void Start();

	void Init();

	WindowSystem *GetWindowSystem(void) { return m_windowSystem; }
	ResourceSystem *GetResourceSystem(void) { return m_resourceSystem; }

	static DragonEngine* Create(void);

	static GameState GetGameState(void) { return m_instance->m_gameState; }
	static GameLoopState GetGameLoopState(void) { return m_instance->m_gameLoopState; }
};