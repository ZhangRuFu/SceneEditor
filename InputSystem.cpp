#include "InputSystem.h"
#include "CommonType.h"

InputSystem::InputSystem(void)
{
	m_keyManager = new KeyManager();
	m_mouseManager = new MouseManager();
}

InputSystem * InputSystem::GetInstance(void)
{
	if (m_instance == nullptr)
		m_instance = new InputSystem();
	return m_instance;
}

bool InputSystem::isKeyDown(int key)
{
	return m_instance->m_keyManager->isKeyDown(key);
}

bool InputSystem::isKeyDown(int key, int ctrlKey)
{
	return m_instance->m_keyManager->isKeyDown(key) && m_instance->m_keyManager->isKeyDown(ctrlKey);
}

bool InputSystem::isKeyUp(int key)
{
	return m_instance->m_keyManager->isKeyUp(key);
}

void InputSystem::KeyDown(int key)
{
	m_keyManager->KeyDown(key);
}

void InputSystem::KeyUp(int key)
{
	m_keyManager->KeyUp(key);
}

void InputSystem::MouseEvent(int x, int y, MouseMotion mouseMotion)
{
	static bool isDown[]{ false, true, false, true, false, false };

	m_mouseManager->SetMousePosition(x, y);

	if (mouseMotion == MouseMotion::MouseMove)
		m_mouseManager->MouseMove();
	else if (isDown[(int)mouseMotion])
		m_mouseManager->MouseKeyDown(mouseMotion);
	else
		m_mouseManager->MouseKeyUp(mouseMotion);
}

void InputSystem::Move(void)
{
	m_mouseManager->Move();
	m_keyManager->Move();
}

void InputSystem::GetMousePosition(int & x, int & y)
{
	m_instance->m_mouseManager->GetMousePosition(x, y);
}

bool InputSystem::isMouseKeyDown(int mouseKey)
{
	return m_instance->m_mouseManager->isMouseKeyDown(mouseKey);
}

bool InputSystem::isMouseKeyUp(int mouseKey)
{
	return m_instance->m_mouseManager->isMouseKeyUp(mouseKey);
}

bool InputSystem::isOnMouseKeyDown(int mouseKey)
{
	return m_instance->m_mouseManager->isOnMouseKeyDown(mouseKey);
}

bool InputSystem::isOnMouseKeyUp(int mouseKey)
{
	return m_instance->m_mouseManager->isOnMouseKeyDown(mouseKey);
}





KeyManager::KeyManager(void)
{
	for (int i = 0; i < m_keyCount; i++)
		m_key[i] = false;
}

bool KeyManager::isKeyDown(int key)
{
	if (key < 0 || key >= m_keyCount)
		return false;
	return m_key[key];
}

bool KeyManager::isKeyUp(int key)
{
	if (key < 0 || key >= m_keyCount)
		return false;
	return !m_key[key];
}

void KeyManager::KeyDown(int key)
{
	m_key[key] = true;
}

void KeyManager::KeyUp(int key)
{
	m_key[key] = false;
}

void KeyManager::Move(void)
{
	//====================================================待添加================================================
}






MouseManager::MouseManager(void)
{
	//所有按键初始均为弹起状态
	for (int i = 0; i < m_mouseKeyCount; i++)
		m_mouseKey[i] = false;
}

void MouseManager::GetMousePosition(int & x, int & y)
{
	x = m_x;
	y = m_y;
}

bool MouseManager::isMouseKeyDown(int mouseKey)
{
	if (mouseKey < 0 || mouseKey >= m_mouseKeyCount)
		return false;
	return m_mouseKey[mouseKey];
}

bool MouseManager::isMouseKeyUp(int mouseKey)
{
	if (mouseKey < 0 || mouseKey >= m_mouseKeyCount)
		return false;
	return !m_mouseKey[mouseKey];
}

bool MouseManager::isOnMouseKeyDown(int mouseKey)
{
	static MouseMotion mouseMap[2]{ MouseMotion::LeftButtonDown, MouseMotion::RightButtonDown };

	if (mouseKey <= MOUSE_UNKNOWN || mouseKey >= MOUSE_KEY_MIDDLE)
		return false;
	return mouseMap[mouseKey - MOUSE_KEY_LEFT] == m_mouseMotion;
}

bool MouseManager::isOnMouseKeyUp(int mouseKey)
{
	static MouseMotion mouseMap[2]{ MouseMotion::LeftButtonUp, MouseMotion::RightButtonUp };

	if (mouseKey <= MOUSE_UNKNOWN || mouseKey >= MOUSE_KEY_MIDDLE)
		return false;
	return mouseMap[mouseKey - MOUSE_KEY_LEFT] == m_mouseMotion;
}

void MouseManager::SetMousePosition(int x, int y)
{
	m_x = x;
	m_y = y;
}

void MouseManager::MouseKeyDown(MouseMotion motion)
{
	static int buttonMap[]{ MOUSE_UNKNOWN, MOUSE_KEY_RIGHT, MOUSE_KEY_RIGHT, MOUSE_KEY_LEFT, MOUSE_KEY_LEFT, MOUSE_KEY_MIDDLE };
	int mouseKey = buttonMap[(int)motion];
	if(mouseKey > 0 && mouseKey < m_mouseKeyCount)
		m_mouseKey[mouseKey] = true;
	m_mouseMotion = motion;
}

void MouseManager::MouseKeyUp(MouseMotion motion)
{
	static int buttonMap[]{ MOUSE_UNKNOWN, MOUSE_KEY_RIGHT, MOUSE_KEY_RIGHT, MOUSE_KEY_LEFT, MOUSE_KEY_LEFT, MOUSE_KEY_MIDDLE };
	int mouseKey = buttonMap[(int)motion];
	if (mouseKey > 0 && mouseKey < m_mouseKeyCount)
		m_mouseKey[mouseKey] = false;
	m_mouseMotion = motion;
}

void MouseManager::MouseMove(void)
{
	m_mouseMotion = MouseMotion::MouseMove;
}

void MouseManager::Move(void)
{
	m_mouseMotion = MouseMotion::NoughtMouse;
}


InputSystem *InputSystem::m_instance = nullptr;