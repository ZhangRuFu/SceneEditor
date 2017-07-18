#pragma once
#include <string>
#include <map>
#include "CommonType.h"
#include "ComponentManager.h"

using std::string;
using std::map;

class Shader;
struct GraphicsBuffer;

/*
*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：Drawer
*	描　　述：所有需要进行绘制的组件必须继承该抽象类
*
*/
class Drawer : public Component
{
protected:
	GraphicsBuffer *m_buffers;
	Shader *m_shader;

public:
	Drawer(string shaderName);
	Shader* GetShader();
	virtual void Draw() = 0;
	virtual void PublicSet() = 0;
	virtual RenderLevel GetRenderLevel() = 0;

protected:
	void Register(void);
};


/*
*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：ModelDrawer
*	描　　述：模型绘制器基类
*
*/
class Model;
class ModelDrawer : public Drawer
{
private:
	static map<int, GraphicsBuffer*> m_buffersMap;

public:
	ModelDrawer(Model *mesh, string shaderName) : Drawer(shaderName) {}

	virtual void Draw() = 0;
	virtual void PublicSet() = 0;
	virtual int GetComponentType(void) = 0;
	virtual GraphicsBuffer* LoadGraphicsBuffer(Model *mesh) = 0;
	virtual RenderLevel GetRenderLevel(void) { return RenderLevel::Entity; }

protected:
	void AddModelBuffer(Model *model, GraphicsBuffer *gb);
	GraphicsBuffer* GetModelBuffer(Model *model);
};