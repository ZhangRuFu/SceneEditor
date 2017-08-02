#include <GL\glew.h>
#include <GLM\gtx\transform.hpp>
#include <GLM\gtc\type_ptr.hpp>
#include "Tiny2D.h"
#include "RenderSystem.h"
#include "ResourceSystem.h"

Tiny2D::Tiny2D(string shaderName) : Drawer(shaderName)
{
	if (m_shapes == nullptr)
		InitShapeData();
	Drawer::m_buffers = m_shapeBuffer;
	Drawer::ChangeRenderLevel(RenderLevel::NonRender);
}


void Tiny2D::DrawLine(vec2 start, vec2 end, vec3 color, int width)
{
	static vec3 point[2];
	point[0] = vec3(start.x, -start.y, m_depth);
	point[1] = vec3(end.x, -end.y, m_depth);
	glLineWidth(width);
	glBindBuffer(GL_ARRAY_BUFFER, Drawer::m_buffers->m_vbo[Shape::Basic2D::Line]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3) * 2, &point);

	glm::mat4 model;
	Drawer::m_shader->SetUniformValue("model", model);
	glUniform3fv(Drawer::m_shader->GetUniformLocation("color"), 1, value_ptr(color));
	glBindVertexArray(Drawer::m_buffers->m_vao[Shape::Basic2D::Line]);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
	glLineWidth(1);
}

void Tiny2D::DrawTriangle(vec2 lt, int width, int height, vec3 color)
{
	glm::mat4 model = translate(vec3(lt.x, -lt.y, m_depth));
	model = scale(model, vec3(width, height, 1.0f));
	Drawer::m_shader->SetUniformValue("model", model);
	glUniform3fv(Drawer::m_shader->GetUniformLocation("color"), 1, value_ptr(color));
	glBindVertexArray(Drawer::m_buffers->m_vao[Shape::Basic2D::Triangle]);
	glDrawElements(GL_TRIANGLES, m_shapes->GetIndexCount(Shape::Basic2D::Triangle), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Tiny2D::DrawRect(vec2 rt, int width, int height, vec3 color)
{
	glm::mat4 model = translate(vec3(rt.x, -rt.y, m_depth));
	model = scale(model, vec3(width, height, 1.0f));
	Drawer::m_shader->SetUniformValue("model", model);
	glUniform3fv(Drawer::m_shader->GetUniformLocation("color"), 1, value_ptr(color));
	glBindVertexArray(Drawer::m_buffers->m_vao[Shape::Basic2D::Rectangle]);
	glDrawElements(GL_TRIANGLES, m_shapes->GetIndexCount(Shape::Basic2D::Rectangle), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Tiny2D::DrawText(const string &str, vec2 positioin, int fontSize, vec3 color)
{
	FontRender::m_shader->Use();
	FontRender::DrawText(str, positioin, fontSize, color, m_depth);
	Drawer::m_shader->Use();
}

void Tiny2D::DrawCircle(vec2 lt, int radius, vec3 color)
{
	glm::mat4 model = translate(vec3(lt.x, -lt.y, m_depth));
	model = scale(model, vec3(radius, radius, 1.0f));
	Drawer::m_shader->SetUniformValue("model", model);
	glUniform3fv(Drawer::m_shader->GetUniformLocation("color"), 1, value_ptr(color));
	glBindVertexArray(Drawer::m_buffers->m_vao[Shape::Basic2D::Circle]);
	glDrawElements(GL_TRIANGLES, m_shapes->GetIndexCount(Shape::Basic2D::Circle), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Tiny2D::DrawRoundRect(vec2 lt, int width, int height, int radius, vec3 color)
{
	int diameter = radius * 2;
	if (diameter > height || diameter > width)
		throw exception("圆角矩形radius > width || height");

	float halfW = width / 2;
	float halfH = height / 2;
	
	glBindBuffer(GL_ARRAY_BUFFER, Drawer::m_buffers->m_vbo[Shape::Basic2D::RoundedRect]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3) * m_shapes->GetVertexCount(Shape::Basic2D::RoundedRect), m_shapes->GetRoundedRectData(width, height, radius));
	
	//Drawer::m_shader->Use();
	glm::mat4 model = translate(vec3(halfW + lt.x, -(halfH + lt.y), m_depth));
	Drawer::m_shader->SetUniformValue("model", model);
	glUniform3fv(Drawer::m_shader->GetUniformLocation("color"), 1, value_ptr(color));
	glBindVertexArray(Drawer::m_buffers->m_vao[Shape::Basic2D::RoundedRect]);
	glDrawElements(GL_TRIANGLES, m_shapes->GetIndexCount(Shape::Basic2D::RoundedRect), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Tiny2D::EnableModel(DrawModel drawModel)
{
	switch (drawModel)
	{
	case DrawModel::FORCE_DEPTH:
		//设置强制通过深度测试
		glDepthFunc(GL_ALWAYS);
		break;
	case DrawModel::NORMAL_DEPTH:
		//正常深度测试
		glDepthFunc(GL_LEQUAL);
		break;
	}
}

void Tiny2D::SetDepth(DrawDepth depth)
{
	m_depth = depth;
}

void Tiny2D::PublicSet()
{
	mat4 windowProjection = ResourceSystem::GetMainCamera()->GenWindowProjectionMatrix(10.0f, -10.0f);
	Drawer::m_shader->SetUniformValue("projection", windowProjection);
	FontRender::m_shader->Use();
	FontRender::PublicSet();
	Drawer::m_shader->Use();
}


void Tiny2D::InitShapeData(void)
{
	//建立缓冲对象
	m_shapes = new Shape();
	m_shapeBuffer = new GraphicsBuffer();
	unsigned int *bufferObject = new unsigned int[3 * Shape::Basic2D::Basic2DCount];
	glGenBuffers(2 * Shape::Basic2D::Basic2DCount, bufferObject);
	glGenVertexArrays(Shape::Basic2D::Basic2DCount, bufferObject + 2 * Shape::Basic2D::Basic2DCount);
	for (int i = 0; i < Shape::Basic2D::Basic2DCount; ++i)
	{
		m_shapeBuffer->m_vbo.push_back(bufferObject[i]);
		m_shapeBuffer->m_ebo.push_back(bufferObject[i + Shape::Basic2D::Basic2DCount]);
		m_shapeBuffer->m_vao.push_back(bufferObject[i + 2 * Shape::Basic2D::Basic2DCount]);
	}
	delete[] bufferObject;
	
	//线段
	BufferData(Shape::Basic2D::Line, BufferUsage::DYNAMIC, true);

	//三角形
	BufferData(Shape::Basic2D::Triangle, BufferUsage::STATIC);

	//矩形
	BufferData(Shape::Basic2D::Rectangle, BufferUsage::STATIC);

	//圆形
	BufferData(Shape::Basic2D::Circle, BufferUsage::STATIC);

	//圆角矩形
	BufferData(Shape::Basic2D::RoundedRect, BufferUsage::DYNAMIC, true);
}

void Tiny2D::BufferData(Shape::Basic2D basic2D, BufferUsage bufferUsage, bool isEmptyData)
{
	static int gl_BufferUsage[3]{ GL_STATIC_DRAW, GL_DYNAMIC_DRAW, GL_STREAM_DRAW };
	glBindVertexArray(m_shapeBuffer->m_vao[basic2D]);
	glBindBuffer(GL_ARRAY_BUFFER, m_shapeBuffer->m_vbo[basic2D]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_shapeBuffer->m_ebo[basic2D]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * m_shapes->GetVertexCount(basic2D), isEmptyData ? nullptr : m_shapes->GetVertexData(basic2D), gl_BufferUsage[bufferUsage]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_shapes->GetIndexCount(basic2D), m_shapes->GetIndexData(basic2D), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}




//======================================Shape===============================

Shape::Shape(void)
{
	Init();
}

const void * Shape::GetRoundedRectData(int width, int height, int radius)
{
	static vector<vec3> position;
	position.clear();

	float halfW = width / 2;
	float halfH = height / 2;
	static vec2 offset[4]{
		vec2(1, 1),
		vec2(-1, 1),
		vec2(-1, -1),
		vec2(1, -1)
	};
	position.push_back(vec3(0.0f, 0.0f, 0.0f));

	for (int i = 0; i < 4; i++)
	{
		int startIndex = i * (m_divideAngle + 1) + 1;
		for (int j = 0; j <= m_divideAngle; j++)
			position.push_back(vec3(radius * m_shapes[Basic2D::RoundedRect].m_position[startIndex + j].x + offset[i].x * (halfW - radius), radius * m_shapes[Basic2D::RoundedRect].m_position[startIndex + j].y + offset[i].y * (halfH - radius), 0.0f));
	}
	return position.data();
}

void Shape::Init(void)
{
	m_shapes.resize(Basic2D::Basic2DCount);

	//线段
	m_shapes[Basic2D::Line].m_position.reserve(2);
	m_shapes[Basic2D::Line].m_position.push_back(vec3(0.0f, 0.0f, 0.0f));
	m_shapes[Basic2D::Line].m_position.push_back(vec3(0.0f, 0.0f, 0.0f));

	//三角形
	m_shapes[Basic2D::Triangle].m_position.reserve(3);
	m_shapes[Basic2D::Triangle].m_position.push_back(vec3(0.0f, 0.5f, 0.0f));
	m_shapes[Basic2D::Triangle].m_position.push_back(vec3(-0.5f, -0.5f, 0.0f));
	m_shapes[Basic2D::Triangle].m_position.push_back(vec3(0.5f, -0.5f, 0.0f));
	Tranglarize(m_shapes[Basic2D::Triangle].m_index, m_shapes[Basic2D::Triangle].m_position.size());

	//矩形
	m_shapes[Basic2D::Rectangle].m_position.reserve(4);
	m_shapes[Basic2D::Rectangle].m_position.push_back(vec3(0.0f, 0.0f, 0.0f));
	m_shapes[Basic2D::Rectangle].m_position.push_back(vec3(0.0f, -1.0f, 0.0f));
	m_shapes[Basic2D::Rectangle].m_position.push_back(vec3(1.0f, -1.0f, 0.0f));
	m_shapes[Basic2D::Rectangle].m_position.push_back(vec3(1.0f, 0.0f, 0.0f));
	Tranglarize(m_shapes[Basic2D::Rectangle].m_index, m_shapes[Basic2D::Rectangle].m_position.size());

	//圆形
	m_shapes[Basic2D::Circle].m_position.push_back(vec3(0.0f + 1.0f, 0.0f - 1.0f, 0.0f));
	for (float angle = 0; angle < 360; angle+=5)
		m_shapes[Basic2D::Circle].m_position.push_back(vec3(cos(radians(angle)) + 1.0f, sin(radians(angle)) - 1.0f, 0.0f));
	Tranglarize(m_shapes[Basic2D::Circle].m_index, m_shapes[Basic2D::Circle].m_position.size(), true);

	//圆角矩形
	
	m_shapes[Basic2D::RoundedRect].m_position.reserve(1 + (m_divideAngle + 1) * 4);
	m_shapes[Basic2D::RoundedRect].m_position.push_back(vec3(0.0, 0.0, 0.0));
	float curAngle = 0;
	float divAngle = 90.0 / m_divideAngle;
	for (int i = 0; i < 4; i++)
	{
		float angle = 0;
		for (int j = 0; j <= m_divideAngle; j++, angle += divAngle)
			m_shapes[Basic2D::RoundedRect].m_position.push_back(vec3(cos(radians(curAngle + angle)), sin(radians(curAngle + angle)), 0.0f));
		curAngle += 90;
	}
	Tranglarize(m_shapes[Basic2D::RoundedRect].m_index, m_shapes[Basic2D::RoundedRect].m_position.size(), true);
}

void Shape::Tranglarize(vector<unsigned int> &index, int count, bool isClose)
{
	for (int i = 1; i < count - 1; i++)
	{
		index.push_back(0);
		index.push_back(i);
		index.push_back(i + 1);
	}
	if (isClose)
	{
		index.push_back(0);
		index.push_back(count - 1);
		index.push_back(1);
	}
}

Shape *Tiny2D::m_shapes = nullptr;
GraphicsBuffer *Tiny2D::m_shapeBuffer = nullptr;