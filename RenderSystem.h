#pragma once
/*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������RenderSystem
*	�衡��������OpenGL��DirectX�ȵײ���Ⱦ����ĳ���Ŀǰֻ֧��OpenGL������Ҫ���ƵĶ�����й���
*
*/
#include <GL\glew.h>
#include <list>
#include <vector>
#include <map>
#include <string>

#include "Drawer.h"
#include "CommonType.h"
#include "UIDrawer.h"

using std::list;
using std::vector;
using std::map;
using std::string;
class DragonEngine;
class Tiny2D;

/*
*	����汾��Dragon Engine v0.1 - 2017��4��9��20:39:36
*	�ࡡ������DrawList
*	�衡�����������б������������Դ��Drawer��Ӧͬһ��shader
*
*/
class DrawerList
{
private:
	int m_shaderIndex;					//shader����
	list<Drawer*> m_drawerList;			//drawer�б�

public:
	DrawerList(int shaderIndex);

	friend class RenderSystem;
};


class RenderSystem
{
	enum ShaderType { VertexShader = GL_VERTEX_SHADER, FragmentShader = GL_FRAGMENT_SHADER };

private:
	DragonEngine *m_engine;
	int m_frameWidth;
	int m_frameHeight;

	vector<DrawerList> m_renderList;					//���ƶ���
	map<string, unsigned int> m_shaderMap;				//Shader��Դ	
	vector<Shader*> m_shaders;
	Tiny2D *m_paint;
	static RenderSystem *m_instance;
private:
	RenderSystem(DragonEngine *engine, int frameWidth, int frameHeight);

public:
	void Draw();
	void ReSize(int frameWidth, int frameHeight);
	void PostRender();
	static void Register(Drawer *drawer);
	static unsigned int CreateTexture(TextureInfo &info);
	static unsigned int CreateCubeTexture(vector<TextureInfo> *info);
	static Shader* LoadShader(string shaderName);
	static RenderSystem* GetInstance(DragonEngine *engine, int frameWidth = 500, int frameHeight = 500);

private:
	bool Init(void);
	void _Register(Drawer *drawer);
	//Shader����
	Shader* CreateShader(string shaderPath, string shaderName);
	int LoadShaderSource(string fileName, char ** source);
	void AddShader(string sourcePath, ShaderType shaderType, GLenum shaderID);
	void RenderSystem::Validate(GLenum shaderID);
};
