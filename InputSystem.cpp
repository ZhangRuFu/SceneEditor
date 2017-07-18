#include "InputSystem.h"

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

void InputSystem::SetMousePosition(int x, int y)
{
	m_mouseManager->SetMousePosition(x, y);
}

void InputSystem::MouseKeyDown(int mouseKey)
{
	m_mouseManager->MouseKeyDown(mouseKey);
}

void InputSystem::MouseKeyUp(int mouseKey)
{
	m_mouseManager->MouseKeyUp(mouseKey);
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

MouseManager::MouseManager(void)
{
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

void MouseManager::SetMousePosition(int x, int y)
{
	m_x = x;
	m_y = y;
}

void MouseManager::MouseKeyDown(int mouseKey)
{
	if(mouseKey > 0 && mouseKey < m_mouseKeyCount)
		m_mouseKey[mouseKey] = true;
}

void MouseManager::MouseKeyUp(int mouseKey)
{
	if (mouseKey > 0 && mouseKey < m_mouseKeyCount)
		m_mouseKey[mouseKey] = false;
}


InputSystem *InputSystem::m_instance = nullptr;