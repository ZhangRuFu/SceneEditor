#pragma once
#include <GLM\common.hpp>
#include <string>
#include <vector>
#include "Drawer.h"
#include "Shader.h"
#include "FreeType.h"
#include "CommonType.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using std::string;
using std::vector;

/*
*	����汾��Dragon Engine v0.1 - 2017��4��9��21:20:38
*	�ࡡ������Shape
*	�衡����������ͼ�����ݣ���Tiny2Dʹ��
*
*/
class Shape
{
	struct ShapeData
	{
		vector<vec3> m_position;			//��������
		vector<unsigned int> m_index;		//��������
	};
public:
	enum Basic2D { Line, Triangle, Rectangle, RoundedRect, Circle, Basic2DCount };
	Shape(void);
	const void* GetVertexData(Shape::Basic2D shapeIndex) { return m_shapes[shapeIndex].m_position.data(); }
	int GetVertexCount(Shape::Basic2D shapeIndex) { return m_shapes[shapeIndex].m_position.size(); };
	const void* GetIndexData(Shape::Basic2D shapeIndex) { return m_shapes[shapeIndex].m_index.data(); };
	int GetIndexCount(Shape::Basic2D shapeIndex) { return m_shapes[shapeIndex].m_index.size(); };
	const void* GetRoundedRectData(int width, int height, int radius);

private:
	void Init(void);
	void Tranglarize(vector<unsigned int> &index, int count, bool isClose = false);

private:
	vector<ShapeData> m_shapes;				//����ͼ��������Ϣ
	const int m_divideAngle = 5;
};


/*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������Tiny2D
*	�衡������2DͼԪ��ͼ��Ļ��ƹ�����, Ҳ��һ��Drawer
*
*/
//����ģʽ
class Tiny2D : public Drawer, public FontRender
{	
private:
	enum BufferUsage { STATIC, DYNAMIC, STREAM };

public:
	enum DrawModel {FORCE_DEPTH, NORMAL_DEPTH};
	enum DrawDepth { Firth = -5, Fourth = -4, Third = -3, Second = -2, First = -1, Normal = 0, NFirst = 1, NSecond = 2, NThird = 3, NFourth = 4, NFifth = 5 };

protected:
	void Register(void) { Drawer::Register(); }

public:
	Tiny2D(string shaderName = "2D");
	void DrawLine(vec2 start, vec2 end, vec3 color = vec3(1.0, 1.0, 1.0), int width = 1);
	void DrawTriangle(vec2 lt, int width, int height, vec3 color);
	void DrawRect(vec2 rt, int width, int height, vec3 color);
	void DrawText(const string &str, vec2 positioin, int fontSize, vec3 color);
	void DrawCircle(vec2 lt, int radius, vec3 color);
	void DrawRoundRect(vec2 lt, int width, int height, int radius, vec3 color);
	void EnableModel(DrawModel drawModel);
	void SetDepth(DrawDepth depth);
	
	virtual void Draw() {}
	virtual void PublicSet();
	virtual RenderLevel GetRenderLevel(void) { return RenderLevel::NonRender; }
	virtual int GetComponentType(void) { return (int)ComponentType::Drawer::Tiny2DDrawer; }

private:
	//���ߺ���
	static void InitShapeData(void);
	static void BufferData(Shape::Basic2D shapeIndex, BufferUsage bufferUsage, bool isEmptyData = false);

private:
	float m_depth;
	static Shape *m_shapes;
	static GraphicsBuffer *m_shapeBuffer;
};
