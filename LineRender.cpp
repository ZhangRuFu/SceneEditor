#include "LineRender.h"
#include "Camera.h"
#include "ResourceSystem.h"
#include "Shader.h"

LineDrawer::LineDrawer() : Drawer("empty")
{
	GLenum vbo = -1, vao = -1;
	vec3 data[2] = { m_origin, m_dest };
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	m_buffers = new GraphicsBuffer();
	m_buffers->m_vao.push_back(vao);
	m_buffers->m_vbo.push_back(vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void LineDrawer::Draw()
{
	glBindVertexArray(m_buffers->m_vao[0]);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
}

void LineDrawer::PublicSet()
{
	Camera *camera = ResourceSystem::GetMainCamera();
	mat4 view = camera->GenViewMatrix();
	mat4 projection = camera->GenProjectionMatrix();

	m_shader->SetUniformValue("view", view);
	m_shader->SetUniformValue("projection", projection);
	m_shader->SetUniformValue("model", mat4());
}

void LineDrawer::setOrigin(vec3 origin)
{
	m_origin = origin;
	glBindBuffer(GL_ARRAY_BUFFER, m_buffers->m_vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3), &m_origin);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void LineDrawer::setDestination(vec3 dest)
{
	m_dest = dest;
	glBindBuffer(GL_ARRAY_BUFFER, m_buffers->m_vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec3), sizeof(vec3), &m_dest);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void LineDrawer::SetRay(const Ray & ray, float len)
{
	setOrigin(ray.getOrigin());
	setDestination(ray.GetPoint(len));
}
