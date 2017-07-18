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

class DragonEngine
{
public:
	enum GameState { PreStart, Gaming, Pause, Finish };

private:
	GameState m_gameState;
	RenderSystem *m_renderSystem;
	ResourceSystem *m_resourceSystem;
	InputSystem *m_inputSystem;
	WindowSystem *m_windowSystem;
	Time *m_time;

public:
	DragonEngine(void);

	void Move();

	void Draw();

	void Start();

	void Init();

	WindowSystem *GetWindowSystem(void) { return m_windowSystem; }

	GameState GetGameState(void) { return m_gameState; }
};