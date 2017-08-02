#include "DragonEngine.h"
#include "Soldier.h"
#include "WoodPlane.h"
#include "Monster.h"
#include "GLFWWindowSystem.h"
#include "Activity.h"
#include "Picker.h"
#include "TransformEditor.h"
#include <Windows.h>
#include <iostream>
#include <exception>

#pragma comment(lib, "winmm.lib")

using std::cout;
using std::endl;

DragonEngine::DragonEngine(void)
{
	string windowName = "SceneEditor-v1.0";
	int frameWidth, frameHeight;
	m_gameState = GameState::PreStart;
	m_gameLoopState = GameLoopState::Ready;
	m_windowSystem = GLFWWindowSystem::GetInstance(800, 800, windowName, this);
	m_windowSystem->GetFrameSize(frameWidth, frameHeight);
	m_renderSystem = RenderSystem::GetInstance(this, frameWidth, frameHeight);
	m_resourceSystem = ResourceSystem::GetInstance(this);
	m_inputSystem = InputSystem::GetInstance();
	m_windowSystem->AssignInput(m_inputSystem);
	m_time = new Time();
	Init();
	cout << "DragonEngine->SUCCESS:DragonEngine初始化完成！" << endl;
}

void DragonEngine::Move()
{
	m_gameLoopState = GameLoopState::LogicMove;
	static unsigned int lastTime = timeGetTime();
	unsigned int currentTime = timeGetTime();
	unsigned int timeSpan = currentTime - lastTime;
	m_time->Elapse(timeSpan);
	lastTime = currentTime;

	if (m_gameState == GameState::Gaming)
	{
		m_resourceSystem->Move();
		m_inputSystem->Move();
	}
}

void DragonEngine::Draw()
{
	m_gameLoopState = GameLoopState::Render;
	m_renderSystem->Draw();
}

void DragonEngine::Start()
{
	//游戏循环开始
	m_gameState = GameState::Gaming;
	cout << "DragonEngine->SUCCESS:DragonEngine开始运行!" << endl;
	m_windowSystem->Start();
}

void DragonEngine::Init()
{
	//DragonActivity *activity = new DragonActivity("DragonActivity");
	//m_windowSystem->AddActivity(activity);
	
	int frameWidth, frameHeight;
	m_windowSystem->GetFrameSize(frameWidth, frameHeight);
	Camera *m_camera = new Camera(frameWidth, frameHeight);
	Soldier *soldier = new Soldier();
	soldier->GetTransform()->Move(vec3(10, 10, 0));
	TranslateAxis *axis = new TranslateAxis();
	
	Picker *picker = new Picker(axis);
	//WoodPlane *woodPlane = new WoodPlane();
	Light *light = new Light();
	light->GetTransform()->Move(vec3(20, 20, 20));
	//woodPlane->GetTransform()->Scale(vec3(50, 50, 50));
	
	m_resourceSystem->Init();
}

DragonEngine * DragonEngine::Create(void)
{
	if (!m_instance)
	{
		m_instance = new DragonEngine();
		return m_instance;
	}
	else
		throw std::exception("DragonEngine Only allowed to be created once!");
}


DragonEngine *DragonEngine::m_instance = nullptr;