#pragma once
/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：DragonEngine
*	描　　述：Dragon游戏引擎实体
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