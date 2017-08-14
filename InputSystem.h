#pragma once
//==========================================================�������Ƶĺ������⣬friend class===========================================================

enum class MouseMotion { NoughtMouse, RightButtonDown, RightButtonUp, LeftButtonDown, LeftButtonUp, MouseMove };
enum class KeyMotion { NoughtKey, KeyDown, KeyUp, KeyRepeat };

/*
*	����:KeyManager
*	���������̹��������Լ��̰���״̬���¼����й���
*	����޸�ʱ�䣺2017��8��2��15:34:30
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
*	����:MouseManager
*	����������������������¼���״̬��λ�ý��й���
*	����޸�ʱ�䣺2017��8��2��15:16:12
*
*/
class MouseManager
{
	friend class InputSystem;
private:
	static const int m_mouseKeyCount = 3;
	int m_x, m_y;
	bool m_mouseKey[m_mouseKeyCount];								//���״̬-����˳ʱ���¼�,true��ʾ���£�false��ʾ����
	MouseMotion m_mouseMotion = MouseMotion::NoughtMouse;			//����¼�-ǿ��ʱЧ��

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
*	����:InputSystem
*	����������ϵͳ���������豸���й���Ŀǰ�����豸Ϊ���̺����
*	����޸�ʱ�䣺2017��8��2��15:16:12
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

	//����
	static bool isKeyDown(int key);
	static bool isKeyDown(int key, int ctrlKey);
	static bool isKeyUp(int key);

	//���
	static bool isMouseKeyDown(int mouseKey);
	static bool isMouseKeyUp(int mouseKey);
	static bool isOnMouseKeyDown(int mouseKey);
	static bool isOnMouseKeyUp(int mouseKey);
	static void GetMousePosition(int &x, int &y);

	void KeyDown(int key);
	void KeyUp(int key);

	void MouseEvent(int x, int y, MouseMotion mouseMotion);

	//�˷���Ӧ����DragonEngine��ResourceSystem��Move֮�����
	void Move(void);
};