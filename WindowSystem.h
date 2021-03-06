#pragma once
/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：WindowSystem
*	描　　述：对不同窗口系统的抽象
*
*/
#include <string>
#include <map>
#include <GLM\glm.hpp>
#include "UIModel.h"
#include "InputSystem.h"
using std::string;
using std::map;

class DragonEngine;
class InputSystem;

/*
*	Event 事件
*
*/

struct Event
{
	//Mouse
	MouseMotion m_mouseMotion;
	glm::ivec2 m_mousePosition;

	//KeyBoard
	KeyMotion m_keyMotion;
	int m_keyCode;					//Key
	unsigned int m_codePoint;		//UTF-32

	bool m_hasCharMsg;
	bool m_hasKeyMsg;
	bool m_hasMouseMsg;

	Event(void);
	Event(int keyCode, KeyMotion keyMotion);
	Event(unsigned int codePoint);
	Event(glm::ivec2 mousePosition, MouseMotion mouseMotion);
	bool isMouseKeyDown(void)
	{
		static const bool isDown[]{ false, true, false, true, false, false };
		return isDown[(int)m_mouseMotion];
	}

	bool isMouseKeyUp(void)
	{
		static const bool isUp[]{ false, false, true, false, true, false };
		return isUp[(int)m_mouseMotion];
	}
};

class WindowSystem : public UIModel
{
protected:
	DragonEngine *m_engine;
	InputSystem *m_input;
	int m_windowWidth;
	int m_windowHeight;
	int m_frameWidth;
	int m_frameHeight;
	string m_windowName;
	map<int, int> m_keyMap;
	KeyMotion m_keyActionMap[3];
	Activity *m_activity;
	

public:
	WindowSystem(int width, int height, string windowName, DragonEngine *engine);
	void GetFrameSize(int &frameWidth, int &frameHeight);
	void AssignInput(InputSystem *input);

public:
	virtual void Start() = 0;
	virtual void Render() = 0;
	virtual void AddActivity(Activity *activity);
	void MouseEvent(int x, int y, MouseMotion mouseMotion);
	void KeyEvent(int key, KeyMotion keyMotion);
	void CharEvent(unsigned int codepoint);
};