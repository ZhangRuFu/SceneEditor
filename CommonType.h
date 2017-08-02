#pragma once
#include <vector>
#include <GL\glew.h>

using std::vector;

enum class Axis{X, Y, Z};
enum BasicMesh {CUBE = 0, PLANE, SPHERE, CONE, BASICMESHCOUNT, BASICMESHMAX = 20};
enum class RenderLevel {NonRender, Background, AfterBackground, Entity, AfterEntity, UI};
enum class GameState { PreStart, Gaming, Pause, Finish };
enum class GameLoopState { Ready, LogicMove, Render };

class TextureInfo
{
	friend class RenderSystem;

private:
	bool m_isAlpha;
	int m_width;
	int m_height;
	unsigned char *m_imageData;

public:
	TextureInfo(bool isAlpha, int width, int height, unsigned char *imageData) : m_isAlpha(isAlpha), m_width(width), m_height(height), m_imageData(imageData) {}
	unsigned char* getImageData(void) { return m_imageData; }
};

struct GraphicsBuffer
{
	vector<GLenum> m_vao;
	vector<GLenum> m_vbo;
	vector<GLenum> m_ebo;
};

enum class RenderModeType { DepthTest };

#define KEY_LBUTTON			1	//Êó±ê×ó¼ü
#define KEY_RBUTTON			2	//Êó±êÓÒ¼ü
#define KEY_CANCEL			3	//Cancel
#define KEY_MBUTTON			4	//Êó±êÖÐ¼ü
#define KEY_XBUTTON1		5
#define KEY_XBUTTON2		6
#define KEY_BACK			8	//Backspace
#define KEY_TAB				9	//Tab
#define KEY_CLEAR			12	//Clear
#define KEY_RETURN			13	//Enter
#define KEY_SHIFT			16	//Shift
#define KEY_CONTROL			17	//Ctrl
#define KEY_MENU			18	//Alt
#define KEY_PAUSE			19	//Pause
#define KEY_CAPITAL			20	//Caps Lock
#define KEY_KANA			21
#define KEY_HANGUL			21
#define KEY_JUNJA			23
#define KEY_FINAL			24
#define KEY_HANJA			25
#define KEY_KANJI			25	//*
#define KEY_ESCAPE			27	//Esc
#define KEY_CONVERT			28
#define KEY_NONCONVERT		29
#define KEY_ACCEPT			30
#define KEY_MODECHANGE		31
#define KEY_SPACE			32	//Space
#define KEY_PRIOR			33	//Page Up
#define KEY_NEXT			34	//Page Down
#define KEY_END				35	//End
#define KEY_HOME			36	//Home
#define KEY_LEFT			37	//Left Arrow
#define KEY_UP				38	//Up Arrow
#define KEY_RIGHT			39	//Right Arrow
#define KEY_DOWN			40	//Down Arrow
#define KEY_SELECT			41	//Select
#define KEY_PRINT			42	//Print
#define KEY_EXECUTES		43	//Execute
#define KEY_SNAPSHOT		44	//Snapshot
#define KEY_INSERT			45	//Insert
#define KEY_DELETE			46	//Delete
#define KEY_HELP			47	//Help
#define KEY_0				48	//0
#define KEY_1				49	//1
#define KEY_2				50	//2
#define KEY_3				51	//3
#define KEY_4				52	//4
#define KEY_5				53	//5
#define KEY_6				54	//6
#define KEY_7				55	//7
#define KEY_8				56	//8
#define KEY_9				57	//9
#define KEY_A				65	//A
#define KEY_B				66	//B
#define KEY_C				67	//C
#define KEY_D				68	//D
#define KEY_E				69	//E
#define KEY_F				70	//F
#define KEY_G				71	//G
#define KEY_H				72	//H
#define KEY_I				73	//I
#define KEY_J				74	//J
#define KEY_K				75	//K
#define KEY_L				76	//L
#define KEY_M				77	//M
#define KEY_N				78	//N
#define KEY_O				79	//O
#define KEY_P				80	//P
#define KEY_Q				81	//Q
#define KEY_R				82	//R
#define KEY_S				83	//S
#define KEY_T				84	//T
#define KEY_U				85	//U
#define KEY_V				86	//V
#define KEY_W				87	//W
#define KEY_X				88	//X
#define KEY_Y				89	//Y
#define KEY_Z				90	//Z
#define KEY_LWIN			91
#define KEY_RWIN			92
#define KEY_APPS			93
#define KEY_SLEEP			95
#define KEY_NUMPAD0			96		//Ð¡¼üÅÌ 0
#define KEY_NUMPAD1			97		//Ð¡¼üÅÌ 1
#define KEY_NUMPAD2			98		//Ð¡¼üÅÌ 2
#define KEY_NUMPAD3			99		//Ð¡¼üÅÌ 3
#define KEY_NUMPAD4			100		//Ð¡¼üÅÌ 4
#define KEY_NUMPAD5			101		//Ð¡¼üÅÌ 5
#define KEY_NUMPAD6			102		//Ð¡¼üÅÌ 6
#define KEY_NUMPAD7			103		//Ð¡¼üÅÌ 7
#define KEY_NUMPAD8			104		//Ð¡¼üÅÌ 8
#define KEY_NUMPAD9			105		//Ð¡¼üÅÌ 9
#define KEY_MULTIPLY		106		//Ð¡¼üÅÌ *
#define KEY_ADD				107		//Ð¡¼üÅÌ +
#define KEY_SEPARATOR		108		//Ð¡¼üÅÌ Enter
#define KEY_SUBTRACT		109		//Ð¡¼üÅÌ -
#define KEY_DECIMAL			110		//Ð¡¼üÅÌ .
#define KEY_DIVIDE			111		//Ð¡¼üÅÌ /
#define KEY_F1				112		//F1
#define KEY_F2				113		//F2
#define KEY_F3				114		//F3
#define KEY_F4				115		//F4
#define KEY_F5				116		//F5
#define KEY_F6				117		//F6
#define KEY_F7				118		//F7
#define KEY_F8				119		//F8
#define KEY_F9				120		//F9
#define KEY_F10				121		//F10
#define KEY_F11				122		//F11
#define KEY_F12				123		//F12
#define KEY_F13				124
#define KEY_F14				125
#define KEY_F15				126
#define KEY_F16				127
#define KEY_F17				128
#define KEY_F18				129
#define KEY_F19				130
#define KEY_F20				131
#define KEY_F21				132
#define KEY_F22				133
#define KEY_F23				134
#define KEY_F24				135
#define KEY_NUMLOCK			144			//Num Lock
#define KEY_SCROLL			145			//Scroll
#define KEY_LSHIFT			160
#define KEY_RSHIFT			161
#define KEY_LCONTROL		162
#define KEY_RCONTROL		163
#define KEY_LMENU			164
#define KEY_RMENU			165
#define KEY_BROWSER_BACK		166
#define KEY_BROWSER_FORWARD		167
#define KEY_BROWSER_REFRESH		168
#define KEY_BROWSER_STOP		169
#define KEY_BROWSER_SEARCH		170
#define KEY_BROWSER_FAVORITES	171
#define KEY_BROWSER_HOME		172
#define KEY_VOLUME_MUTE			173			//VolumeMute
#define KEY_VOLUME_DOWN			174			//VolumeDown
#define KEY_VOLUME_UP			175			//VolumeUp
#define KEY_MEDIA_NEXT_TRACK	176
#define KEY_MEDIA_PREV_TRACK	177
#define KEY_MEDIA_STOP			178
#define KEY_MEDIA_PLAY_PAUSE	179
#define KEY_LAUNCH_MAIL			180
#define KEY_LAUNCH_MEDIA_SELECT	181
#define KEY_LAUNCH_APP1			182
#define KEY_LAUNCH_APP2			183
#define KEY_OEM_1				186			//; :
#define KEY_OEM_PLUS			187			//= +
#define KEY_OEM_COMMA			188			//, <
#define KEY_OEM_MINUS			189			//- _
#define KEY_OEM_PERIOD			190			//. >
#define KEY_OEM_2				191			/// ?
#define KEY_OEM_3				192			//` ~
#define KEY_OEM_4				219			//[{
#define KEY_OEM_5				220			//\ |
#define KEY_OEM_6				221			//] }
#define KEY_OEM_7				222			//' " 
#define KEY_OEM_8				223
#define KEY_OEM_102				226			
#define KEY_PACKET				231
#define KEY_PROCESSKEY			229
#define KEY_ATTN				246
#define KEY_CRSEL				247
#define KEY_EXSEL				248
#define KEY_EREOF				249
#define KEY_PLAY				250
#define KEY_ZOOM				251
#define KEY_NONAME				252
#define KEY_PA1					253
#define KEY_OEM_CLEAR			254

#define MOUSE_UNKNOWN			0
#define MOUSE_KEY_LEFT			1
#define MOUSE_KEY_RIGHT			2
#define MOUSE_KEY_MIDDLE		3
