#pragma once
//==========================================================这个类设计的很有问题，friend class===========================================================
class KeyManager
{
	friend class InputSystem;
private:
	static const int m_keyCount = 256;
	bool m_key[m_keyCount];

private:
	KeyManager(void);

	bool isKeyDown(int key);
	bool isKeyUp(int key);

public:
	void KeyDown(int key);
	void KeyUp(int key);
};

class MouseManager
{
	friend class InputSystem;
private:
	static const int m_mouseKeyCount = 3;
	int m_x, m_y;
	bool m_mouseKey[m_mouseKeyCount];

private:
	MouseManager(void);

public:
	void GetMousePosition(int &x, int &y);
	bool isMouseKeyDown(int mouseKey);
	bool isMouseKeyUp(int mouseKey);

public:
	void SetMousePosition(int x, int y);
	void MouseKeyDown(int mouseKey);
	void MouseKeyUp(int mouseKey);
};

class InputSystem
{
private:
	static InputSystem *m_instance;

private:
	KeyManager *m_keyManager;
	MouseManager *m_mouseManager;

private:
	InputSystem(void);

public:
	static InputSystem* GetInstance(void);

public:
	static bool isKeyDown(int key);
	static bool isKeyUp(int key);
	static void GetMousePosition(int &x, int &y);
	static bool isMouseKeyDown(int mouseKey);
	static bool isMouseKeyUp(int mouseKey);

public:
	void KeyDown(int key);
	void KeyUp(int key);
	void SetMousePosition(int x, int y);
	void MouseKeyDown(int mouseKey);
	void MouseKeyUp(int mouseKey);
};