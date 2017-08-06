#include <iostream>
#include <stdarg.h>
#include "RenderSystem.h"
#include "Shader.h"
#include "Tiny2D.h"
#include "FreeType.h"
#include "DragonEngine.h"
#include "Activity.h"


#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")

using std::cout;
using std::endl;

RenderSystem::RenderSystem(DragonEngine *engine, int frameWidth, int frameHeight)
{
	m_engine = engine;
	m_frameWidth = frameWidth;
	m_frameHeight = frameHeight;
	bool result = Init();
	if (result)
		cout << "DragonEngine->SUCCESS:渲染引擎初始化完成!" << endl;
	else
		cout << "DragonEngine->ERROR:RenderSystem::Init():渲染引擎初始化失败!" << endl;
}

RenderSystem* RenderSystem::GetInstance(DragonEngine *engine, int frameWidth, int frameHeight)
{
	if (m_instance == nullptr)
	{
		m_instance = new RenderSystem(engine, frameWidth, frameHeight);
		m_instance->PostRender();
	}
	return m_instance;
}

bool RenderSystem::Init()
{
	//OpenGL版本号
	const GLubyte *version = glGetString(GL_VERSION);
	cout << "OpenGL版本:" << version << endl;

	//GLEW初始化
	GLenum result = glewInit();
	if (result != GLEW_OK)
	{
		cout << "DragonEngine->ERROR:GLEW初始化错误!错误信息:" << glewGetErrorString(result) << endl;
		return false;
	}
	else
		cout << "DragonEngine->SUCCESS:GLEW初始化成功!" << endl;

	//OpenGL初始化
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glViewport(0, 0, m_frameWidth, m_frameHeight);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);

	//加载必要Shader=========================改成XML加载====================================
	CreateShader("E:\\OpenGL\\Shader\\Assimp\\", "AssimpModel");
	CreateShader("E:\\OpenGL\\Shader\\ModelRender\\PureColor\\", "SimpleModel");
	CreateShader("E:\\OpenGL\\Shader\\ModelRender\\Animation\\", "AnimationModel");
	CreateShader("E:\\OpenGL\\Shader\\2D\\", "2D");
	CreateShader("E:\\OpenGL\\Shader\\Font\\", "font");
	CreateShader("E:\\OpenGL\\Shader\\", "empty");

	//字体加载
	FontRender::Init();

	return true;
}

void RenderSystem::Draw()
{
	m_engine->GetResourceSystem()->ReadyRender();

	static list<Drawer*> afterEntity;
	afterEntity.clear();

	//背景暂时为纯色背景
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Entity渲染
	bool needSetRenderMode = false;
	for (unsigned int i = 0; i < m_renderList.size(); i++)
	{
		Shader *shader = m_shaders[m_renderList[i].m_shaderIndex];
		shader->Use();
		list<Drawer*> &drawerList = m_renderList[i].m_drawerList;
		for (list<Drawer*>::iterator iterator = drawerList.begin(); iterator != drawerList.end(); iterator++)
		{
			//=====================================PublicSet多次！=============================================
			if (iterator == drawerList.begin())
				(*iterator)->PublicSet();

			if (!(*iterator)->IsEnable())
				continue;

			if ((*iterator)->GetRenderLevel() == RenderLevel::AfterEntity)
			{
				afterEntity.push_back(*iterator);
				continue;
			}

			needSetRenderMode = (*iterator)->NeedSetRenderMode();
			if (needSetRenderMode)
				(*iterator)->SetRenderMode();
			
			(*iterator)->Draw();

			if (needSetRenderMode)
				(*iterator)->SetRenderMode();
		}
	}

	//AfterEntity渲染
	list<Drawer*>::iterator i = afterEntity.begin();
	while (i != afterEntity.end())
	{
		needSetRenderMode = (*i)->NeedSetRenderMode();
		if (needSetRenderMode)
			(*i)->SetRenderMode();

		//=====================================PublicSet多次！=============================================
		(*i)->PublicSet();
		(*i)->Draw();

		if (needSetRenderMode)
			(*i)->SetRenderMode();
	}


	//UI绘制
	Activity *activity = m_engine->GetWindowSystem()->GetActive();
	if (activity == nullptr)
		return;
	glClearDepth(0.5);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_paint->Drawer::GetShader()->Use();
	m_paint->PublicSet();
	activity->OnDraw(m_paint);
	glClearDepth(1.0);
}

void RenderSystem::ReSize(int frameWidth, int frameHeight)
{
	m_frameWidth = frameWidth; m_frameHeight = frameHeight;
	glViewport(0, 0, m_frameWidth, m_frameHeight);
}

void RenderSystem::PostRender()
{
	m_paint = new Tiny2D();
}

void RenderSystem::_Register(Drawer *drawer)
{
	if (drawer == nullptr)
	{
		cout << "DragonEngine->ERROR:RenderSystem::Register, 待注册的Drawer为null" << endl;
		return;
	}
	string shaderName = drawer->GetShader()->GetShaderName();

	//Shader一定存在
	unsigned int index = m_shaderMap[shaderName];

	//找到shader对应的DrawList
	for (int i = 0; i < m_renderList.size(); i++)
		if (m_renderList[i].m_shaderIndex == index)
		{
			m_renderList[i].m_drawerList.push_back(drawer);
			return;
		}

	//建立绘制项
	vector<DrawerList>::iterator newPos;
	/*if (drawer->GetRenderLevel() == RenderLevel::UI)
	{
		newPos = --m_renderList.end();
		(*newPos).m_shaderIndex = index;
	}
	else*/
	m_renderList.push_back(DrawerList(index));			//.insert(--m_renderList.end(), DrawerList(index));
	m_renderList.back().m_drawerList.push_back(drawer);

}

void RenderSystem::Register(Drawer *drawer)
{
	m_instance->_Register(drawer);
}

unsigned int RenderSystem::CreateTexture(TextureInfo &info)
{
	GLenum texID;
	GLint format = info.m_isAlpha ? GL_RGBA : GL_RGB;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, info.m_isAlpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, info.m_isAlpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, format, info.m_width, info.m_height, 0, format, GL_UNSIGNED_BYTE, info.m_imageData);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texID;
}

unsigned int RenderSystem::CreateCubeTexture(vector<TextureInfo> *info)
{
	GLenum cubeTex;
	if (info->size() != 6)
		return -1;

	glGenTextures(1, &cubeTex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);
	for (unsigned int i = 0; i < info->size(); i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, (*info)[i].m_width, (*info)[i].m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, (*info)[i].m_imageData);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return cubeTex;
}

Shader* RenderSystem::LoadShader(string shaderName)
{
	if (m_instance->m_shaderMap.find(shaderName) != m_instance->m_shaderMap.end())
		return m_instance->m_shaders[m_instance->m_shaderMap[shaderName]];
	cout << "DragonEngine->ERROR:RenderSystem::LoadShader:未知的着色器<" << shaderName << ">\n";
	return nullptr;
}

Shader* RenderSystem::CreateShader(string shaderPath, string shaderName)
{
	GLenum shaderID = glCreateProgram();
	if (!shaderID)
	{
		cout << "DragonEngine->ERROR:RenderSystem::CreateShader:着色器程序创建失败\n";
		return false;
	}
	string shaderFullPath = shaderPath + shaderName;
	AddShader(shaderFullPath + ".vs", ShaderType::VertexShader, shaderID);
	AddShader(shaderFullPath + ".fs", ShaderType::FragmentShader, shaderID);
	Validate(shaderID);
	Shader *shader = new Shader(shaderID, shaderName);

	//注册Shader
	m_shaderMap[shaderName] = m_shaders.size();
	m_shaders.push_back(shader);
	cout << "DragonEngine->SUCCESS:已加载[" << shaderName << "]着色器\n";
	return shader;
}

int RenderSystem::LoadShaderSource(string fileName, char ** source)
{
	FILE *fp = nullptr;
	errno_t result = fopen_s(&fp, fileName.c_str(), "r");
	if (result)
		cout << "Shader文件打开错误!\n";

	fseek(fp, 0, SEEK_END);
	int len = ftell(fp);
	rewind(fp);
	*source = new char[len + 1]();
	fread(*source, len, 1, fp);
	len = strlen(*source);
	fclose(fp);
	return len;
}

void RenderSystem::AddShader(string sourcePath, ShaderType shaderType, GLenum shaderID)
{
	static char* shaderName[2]{ "片段", "顶点" };
	GLenum subShaderID = glCreateShader(shaderType);
	if (!subShaderID)
	{
		cout << "DragonEngine->ERROR:RenderSystem::AddShader, 子着色器创建失败";
		return;
	}

	char *source = nullptr;
	int len = LoadShaderSource(sourcePath, &source);
	glShaderSource(subShaderID, 1, &source, &len);
	glCompileShader(subShaderID);
	GLint isSuccess = 0;
	char info[128];
	glGetShaderiv(subShaderID, GL_COMPILE_STATUS, &isSuccess);
	if (!isSuccess)
	{
		glGetShaderInfoLog(subShaderID, sizeof(info), nullptr, info);
		cout << "DragonEngine->ERROR:RenderSystem::AddShader, "<< shaderName[shaderType - ShaderType::FragmentShader] << "着色器编译错误，错误信息:\n" << info << endl;
		cout << "着色器路径:" << sourcePath << endl;
		return;
	}
	glAttachShader(shaderID, subShaderID);
	delete[] source;
}

void RenderSystem::Validate(GLenum shaderID)
{
	GLint isSuccess = 0;
	char info[128];
	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &isSuccess);
	if (!isSuccess)
	{
		glGetProgramInfoLog(shaderID, 512, NULL, info);
		cout << "着色器链接错误，错误信息:\n" << info << endl;
	}
	glValidateProgram(shaderID);
}

RenderSystem * RenderSystem::m_instance = nullptr;

DrawerList::DrawerList(int shaderIndex)
{
	m_shaderIndex = shaderIndex;
}
