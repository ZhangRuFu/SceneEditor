#include <iostream>
#include "GLFWWindowSystem.h"
#include "DragonEngine.h"
#include "CommonType.h"
#include "InputSystem.h"

#pragma comment(lib, "glfw3.lib")

using std::cout;
using std::endl;

GLFWWindowSystem::GLFWWindowSystem(int width, int height, string windowName, DragonEngine *engine) : WindowSystem(width, height, windowName, engine)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, windowName.c_str(), nullptr, nullptr);
	if (m_window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(m_window);
	
	glfwGetFramebufferSize(m_window, &m_frameWidth, &m_frameHeight);
	
	//键盘动作映射
	m_keyActionMap[GLFW_PRESS] = KeyMotion::KeyDown;
	m_keyActionMap[GLFW_RELEASE] = KeyMotion::KeyUp;
	m_keyActionMap[GLFW_REPEAT] = KeyMotion::KeyRepeat;

	//键盘映射
	m_keyMap[GLFW_KEY_APOSTROPHE] = KEY_OEM_7;
	m_keyMap[GLFW_KEY_COMMA] = KEY_OEM_COMMA;
	m_keyMap[GLFW_KEY_MINUS] = KEY_OEM_MINUS;
	m_keyMap[GLFW_KEY_PERIOD] = KEY_OEM_PERIOD;
	m_keyMap[GLFW_KEY_SLASH] = KEY_OEM_2;
	m_keyMap[GLFW_KEY_SEMICOLON] = KEY_OEM_1;
	m_keyMap[GLFW_KEY_EQUAL] = KEY_OEM_PLUS;
	m_keyMap[GLFW_KEY_LEFT_BRACKET] = KEY_OEM_4;
	m_keyMap[GLFW_KEY_BACKSLASH] = KEY_OEM_5;
	m_keyMap[GLFW_KEY_RIGHT_BRACKET] = KEY_OEM_6;
	m_keyMap[GLFW_KEY_ESCAPE] = KEY_ESCAPE;
	m_keyMap[GLFW_KEY_ENTER] = KEY_RETURN;
	m_keyMap[GLFW_KEY_TAB] = KEY_TAB;
	m_keyMap[GLFW_KEY_BACKSPACE] = KEY_BACK;
	m_keyMap[GLFW_KEY_INSERT] = KEY_INSERT;
	m_keyMap[GLFW_KEY_DELETE] = KEY_DELETE;
	m_keyMap[GLFW_KEY_RIGHT] = KEY_RIGHT;
	m_keyMap[GLFW_KEY_LEFT] = KEY_LEFT;
	m_keyMap[GLFW_KEY_DOWN] = KEY_DOWN;
	m_keyMap[GLFW_KEY_UP] = KEY_UP;
	m_keyMap[GLFW_KEY_PAGE_UP] = KEY_PRIOR;
	m_keyMap[GLFW_KEY_PAGE_DOWN] = KEY_NEXT;
	m_keyMap[GLFW_KEY_HOME] = KEY_HOME;
	m_keyMap[GLFW_KEY_END] = KEY_END;
	m_keyMap[GLFW_KEY_CAPS_LOCK] = KEY_CAPITAL;
	m_keyMap[GLFW_KEY_SCROLL_LOCK] = KEY_SCROLL;
	m_keyMap[GLFW_KEY_NUM_LOCK] = KEY_NUMLOCK;
	m_keyMap[GLFW_KEY_PRINT_SCREEN] = KEY_SNAPSHOT;
	m_keyMap[GLFW_KEY_F1] = KEY_F1;
	m_keyMap[GLFW_KEY_F2] = KEY_F2;
	m_keyMap[GLFW_KEY_F3] = KEY_F3;
	m_keyMap[GLFW_KEY_F4] = KEY_F4;
	m_keyMap[GLFW_KEY_F5] = KEY_F5;
	m_keyMap[GLFW_KEY_F6] = KEY_F6;
	m_keyMap[GLFW_KEY_F7] = KEY_F7;
	m_keyMap[GLFW_KEY_F8] = KEY_F8;
	m_keyMap[GLFW_KEY_F9] = KEY_F9;
	m_keyMap[GLFW_KEY_F10] = KEY_F10;
	m_keyMap[GLFW_KEY_F11] = KEY_F11;
	m_keyMap[GLFW_KEY_F12] = KEY_F12;
	m_keyMap[GLFW_KEY_F13] = KEY_F13;
	m_keyMap[GLFW_KEY_F14] = KEY_F14;
	m_keyMap[GLFW_KEY_F15] = KEY_F15;
	m_keyMap[GLFW_KEY_F16] = KEY_F16;
	m_keyMap[GLFW_KEY_F17] = KEY_F17;
	m_keyMap[GLFW_KEY_F18] = KEY_F18;
	m_keyMap[GLFW_KEY_F19] = KEY_F19;
	m_keyMap[GLFW_KEY_F20] = KEY_F20;
	m_keyMap[GLFW_KEY_F21] = KEY_F21;
	m_keyMap[GLFW_KEY_F22] = KEY_F22;
	m_keyMap[GLFW_KEY_F23] = KEY_F23;
	m_keyMap[GLFW_KEY_F24] = KEY_F24;
	m_keyMap[GLFW_KEY_KP_0] = KEY_NUMPAD0;
	m_keyMap[GLFW_KEY_KP_1] = KEY_NUMPAD1;
	m_keyMap[GLFW_KEY_KP_2] = KEY_NUMPAD2;
	m_keyMap[GLFW_KEY_KP_3] = KEY_NUMPAD3;
	m_keyMap[GLFW_KEY_KP_4] = KEY_NUMPAD4;
	m_keyMap[GLFW_KEY_KP_5] = KEY_NUMPAD5;
	m_keyMap[GLFW_KEY_KP_6] = KEY_NUMPAD6;
	m_keyMap[GLFW_KEY_KP_7] = KEY_NUMPAD7;
	m_keyMap[GLFW_KEY_KP_8] = KEY_NUMPAD8;
	m_keyMap[GLFW_KEY_KP_9] = KEY_NUMPAD9;
	m_keyMap[GLFW_KEY_KP_DECIMAL] = KEY_DECIMAL;
	m_keyMap[GLFW_KEY_KP_DIVIDE] = KEY_DIVIDE;
	m_keyMap[GLFW_KEY_KP_MULTIPLY] = KEY_MULTIPLY;
	m_keyMap[GLFW_KEY_KP_SUBTRACT] = KEY_SUBTRACT;
	m_keyMap[GLFW_KEY_KP_ADD] = KEY_ADD;
	m_keyMap[GLFW_KEY_KP_ENTER] = KEY_SEPARATOR;
	m_keyMap[GLFW_KEY_LEFT_SHIFT] = KEY_LSHIFT;
	m_keyMap[GLFW_KEY_LEFT_CONTROL] = KEY_LCONTROL;
	m_keyMap[GLFW_KEY_LEFT_ALT] = KEY_LMENU;
	m_keyMap[GLFW_KEY_LEFT_SUPER] = KEY_LWIN;
	m_keyMap[GLFW_KEY_RIGHT_SHIFT] = KEY_RSHIFT;
	m_keyMap[GLFW_KEY_RIGHT_CONTROL] = KEY_RCONTROL;
	m_keyMap[GLFW_KEY_RIGHT_ALT] = KEY_RMENU;
	m_keyMap[GLFW_KEY_RIGHT_SUPER] = KEY_RWIN;
	m_keyMap[GLFW_KEY_MENU] = KEY_MENU;


	glfwSetKeyCallback(m_window, KeyEvent);
	glfwSetMouseButtonCallback(m_window, MouseEvent);
	glfwSetCursorPosCallback(m_window, MouseMoveEvent);
	glfwSetWindowRefreshCallback(m_window, Render);
	glfwSetCharCallback(m_window, CharEvent);
}

void GLFWWindowSystem::KeyEvent(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	int engineKey = 0;
	KeyMotion keymotion = KeyMotion::NoughtKey;
	if (key == 32 || (key >= 48 && key <= 57) || (key >= 65 && key <= 90))
		engineKey = key;
	else
	{
		if (m_glfwInstance->m_keyMap.find(key) != m_glfwInstance->m_keyMap.end())
			engineKey = m_glfwInstance->m_keyMap[key];
	}

	keymotion = m_glfwInstance->m_keyActionMap[action];
	m_glfwInstance->WindowSystem::KeyEvent(engineKey, keymotion);//父子之间同名函数无法发生重载
}

void GLFWWindowSystem::MouseEvent(GLFWwindow *window, int button, int action, int mode)
{
	double curX, curY;
	glfwGetCursorPos(m_glfwInstance->m_window, &curX, &curY);
	if (action == GLFW_PRESS)
	{
		m_glfwInstance->WindowSystem::MouseEvent((int)curX, (int)curY, button==GLFW_MOUSE_BUTTON_LEFT ? MouseMotion::LeftButtonDown : MouseMotion::RightButtonDown);
		if (button == GLFW_MOUSE_BUTTON_RIGHT)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else if (action == GLFW_RELEASE)
	{
		m_glfwInstance->WindowSystem::MouseEvent((int)curX, (int)curY, button == GLFW_MOUSE_BUTTON_LEFT ? MouseMotion::LeftButtonUp : MouseMotion::RightButtonUp);
		if (button == GLFW_MOUSE_BUTTON_RIGHT)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void GLFWWindowSystem::MouseMoveEvent(GLFWwindow *window, double x, double y)
{
	m_glfwInstance->WindowSystem::MouseEvent((int)x, (int)y, MouseMotion::MouseMove);
}

void GLFWWindowSystem::CharEvent(GLFWwindow * window, unsigned int codepoint)
{
	//GLFW的字符输入事件
	m_glfwInstance->WindowSystem::CharEvent(codepoint);
}

void GLFWWindowSystem::Render(GLFWwindow * window)
{
	m_glfwInstance->Render();
}

GLFWWindowSystem * GLFWWindowSystem::GetInstance(int width, int height, string windowName, DragonEngine *engine)
{
	if (m_glfwInstance == nullptr)
		m_glfwInstance = new GLFWWindowSystem(width, height, windowName, engine);
	return m_glfwInstance;
}

void GLFWWindowSystem::Start()
{
	while (!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();
		Render();
	}
	glfwTerminate();
}

void GLFWWindowSystem::Render()
{
	if (m_engine && m_engine->GetGameState() == GameState::Gaming)
	{
		m_engine->Move();
		m_engine->Draw();
		glfwSwapBuffers(m_window);
	}
}

GLFWWindowSystem *GLFWWindowSystem::m_glfwInstance = nullptr;