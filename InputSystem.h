#pragma once
//==========================================================这个类设计的很有问题，friend class===========================================================

enum class MouseMotion { NoughtMouse, RightButtonDown, RightButtonUp, LeftButtonDown, LeftButtonUp, MouseMove };
enum class KeyMotion { NoughtKey, KeyDown, KeyUp, KeyRepeat };

/*
*	类名:KeyManager
*	描述：键盘管理器，对键盘按键状态、事件进行管理
*	最后修改时间：2017年8月2日15:34:30
*
*/
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

	void Move(void);
};

/*
*	类名:MouseManager
*	描述：鼠标管理器，对鼠标事件、状态、位置进行管理
*	最后修改时间：2017年8月2日15:16:12
*
*/
class MouseManager
{
	friend class InputSystem;
private:
	static const int m_mouseKeyCount = 3;
	int m_x, m_y;
	bool m_mouseKey[m_mouseKeyCount];								//鼠标状态-并非顺时的事件,true表示按下，false表示弹起
	MouseMotion m_mouseMotion = MouseMotion::NoughtMouse;			//鼠标事件-强调时效性

private:
	MouseManager(void);

public:
	void GetMousePosition(int &x, int &y);
	bool isMouseKeyDown(int mouseKey);
	bool isMouseKeyUp(int mouseKey);
	bool isOnMouseKeyDown(int mouseKey);
	bool isOnMouseKeyUp(int mouseKey);

	void SetMousePosition(int x, int y);
	void MouseKeyDown(MouseMotion motion);
	void MouseKeyUp(MouseMotion motion);
	void MouseMove(void);

	
	void Move(void);
};


/*
*	类名:InputSystem
*	描述：输入系统，对输入设备进行管理。目前输入设备为键盘和鼠标
*	最后修改时间：2017年8月2日15:16:12
*
*/
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

	//键盘
	static bool isKeyDown(int key);
	static bool isKeyDown(int key, int ctrlKey);
	static bool isKeyUp(int key);

	//鼠标
	static bool isMouseKeyDown(int mouseKey);
	static bool isMouseKeyUp(int mouseKey);
	static bool isOnMouseKeyDown(int mouseKey);
	static bool isOnMouseKeyUp(int mouseKey);
	static void GetMousePosition(int &x, int &y);

	void KeyDown(int key);
	void KeyUp(int key);

	void MouseEvent(int x, int y, MouseMotion mouseMotion);

	//此方法应该在DragonEngine的ResourceSystem的Move之后调用
	void Move(void);
};