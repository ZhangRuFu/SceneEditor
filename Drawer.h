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
*	����汾��Dragon Engine v0.1;
*	�ࡡ������Drawer
*	�衡������������Ҫ���л��Ƶ��������̳иó�����
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
*	����汾��Dragon Engine v0.1;
*	�ࡡ������ModelDrawer
*	�衡������ģ�ͻ���������
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