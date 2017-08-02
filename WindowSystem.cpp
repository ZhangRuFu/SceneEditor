#include "WindowSystem.h"
#include "InputSystem.h"
#include "CommonType.h"
#include "Activity.h"

WindowSystem::WindowSystem(int width, int height, string windowName, DragonEngine *engine) : m_windowWidth(width), m_windowHeight(height), m_windowName(windowName)
{
	m_engine = engine;
	m_activity = nullptr;
}

void WindowSystem::GetFrameSize(int & frameWidth, int & frameHeight)
{
	frameWidth = m_frameWidth;
	frameHeight = m_frameHeight;
}

void WindowSystem::AssignInput(InputSystem * input)
{
	m_input = input;
}

void WindowSystem::AddActivity(Activity * activity)
{
	UIModel::AddActivity(activity);
	activity->OnCreate();
	activity->OnMeasure(m_frameWidth, m_frameHeight);
	activity->OnPosit(0, 0);
	m_activity = GetActive();
}

void WindowSystem::MouseEvent(int x, int y, MouseMotion mouseMotion)
{
	m_input->MouseEvent(x, y, mouseMotion);
	
	if(m_activity != nullptr)
		m_activity->AcceptEvent(ivec2(x, y), mouseMotion);
}

//按键事件-保存至InputSystem，供Entity使用
void WindowSystem::KeyEvent(int key, KeyMotion keyMotion)
{
	if (keyMotion == KeyMotion::KeyDown)
		m_input->KeyDown(key);
	else if (keyMotion == KeyMotion::KeyUp)
		m_input->KeyUp(key);
	if (m_activity != nullptr)
		m_activity->AcceptEvent(key, keyMotion);
}

//字符消息-分发至Activity，供UI使用
void WindowSystem::CharEvent(unsigned int codepoint)
{
	if (m_activity != nullptr)
		m_activity->AcceptEvent(codepoint);
}

Event::Event(void) : m_mousePosition(0, 0)
{
	m_mouseMotion = MouseMotion::NoughtMouse;
	m_keyMotion = KeyMotion::NoughtKey;
	m_codePoint = 0;
	m_keyCode = 0;
	m_hasCharMsg = false;
	m_hasKeyMsg = false;
}

Event::Event(int keyCode, KeyMotion keyMotion)
{
	m_hasKeyMsg = true;
	m_keyCode = keyCode;
	m_keyMotion = keyMotion;

	m_hasCharMsg = false;
	m_hasMouseMsg = false;
	m_codePoint = 0;
}

Event::Event(unsigned int codePoint)
{
	m_hasCharMsg = true;
	m_codePoint = codePoint;

	m_hasKeyMsg = m_hasMouseMsg = false;
	m_keyCode = 0;

}

Event::Event(glm::ivec2 mousePosition, MouseMotion mouseMotion)
{
	m_hasMouseMsg = true;
	m_mousePosition = mousePosition;

	m_hasCharMsg = m_hasKeyMsg = false;
	m_codePoint = 0;
	m_keyCode = 0;
}
