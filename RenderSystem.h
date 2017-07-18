#pragma once
/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：RenderSystem
*	描　　述：对OpenGL、DirectX等底层渲染引擎的抽象，目前只支持OpenGL，对需要绘制的对象进行管理
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
*	引擎版本：Dragon Engine v0.1 - 2017年4月9日20:39:36
*	类　　名：DrawList
*	描　　述：绘制列表，多个待绘制资源的Drawer对应同一个shader
*
*/
class DrawerList
{
private:
	int m_shaderIndex;					//shader索引
	list<Drawer*> m_drawerList;			//drawer列表

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

	vector<DrawerList> m_renderList;					//绘制队列
	map<string, unsigned int> m_shaderMap;				//Shader资源	
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
	//Shader加载
	Shader* CreateShader(string shaderPath, string shaderName);
	int LoadShaderSource(string fileName, char ** source);
	void AddShader(string sourcePath, ShaderType shaderType, GLenum shaderID);
	void RenderSystem::Validate(GLenum shaderID);
};
