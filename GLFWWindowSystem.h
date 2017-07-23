#pragma once
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "WindowSystem.h"
using std::string;

class GLFWWindowSystem : public WindowSystem
{
private:
	GLFWwindow *m_window;
	static GLFWWindowSystem *m_glfwInstance;

private:
	GLFWWindowSystem(int width, int height, string windowName, DragonEngine *engine);
	static void KeyEvent(GLFWwindow *window, int key, int scancode, int action, int mode);
	static void MouseEvent(GLFWwindow *window, int button, int action, int mode);
	static void MouseMoveEvent(GLFWwindow *window, double x, double y);
	static void CharEvent(GLFWwindow *window, unsigned int codepoint);
	static void Render(GLFWwindow *window);
	

public:
	void Start();
	void Render();
	static GLFWWindowSystem* GetInstance(int width, int height, string windowName, DragonEngine *engine);
};
