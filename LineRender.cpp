#include "LineRender.h"
#include "Camera.h"
#include "ResourceSystem.h"
#include "Shader.h"
#include "Entity.h"

LineDrawer::LineDrawer(void) : Drawer("empty")
{
	ChangeRenderLevel(RenderLevel::Entity);

	//VBO
	GLenum vbo = -1, vao = -1;
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	m_buffers = new GraphicsBuffer();
	m_buffers->m_vao.push_back(vao);
	m_buffers->m_vbo.push_back(vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * 2, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void LineDrawer::SetEntity(GameEntity & entity)
{
	Component::SetEntity(entity);
	m_transform = entity.GetTransform();
	Register();
}

void LineDrawer::Draw()
{
	if (m_isChanged)
	{
		//¸üÐÂ¶¥µã»º³å
		glBindBuffer(GL_ARRAY_BUFFER, m_buffers->m_vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * m_lineVertics.size(), m_lineVertics.data(), GL_DYNAMIC_DRAW);
		m_isChanged = false;
	}
	if (m_transform != nullptr)
	{
		mat4 model = m_transform->GetModelMatrix();
		m_shader->SetUniformValue("model", model);
	}
	else
		m_shader->SetUniformValue("model", mat4());
	glBindVertexArray(m_buffers->m_vao[0]);
	glDrawArrays(GL_LINES, 0, m_lineVertics.size());
	glBindVertexArray(0);
}

void LineDrawer::PublicSet()
{
	Camera *camera = ResourceSystem::GetMainCamera();
	mat4 view = camera->GenViewMatrix();
	mat4 projection = camera->GenProjectionMatrix();

	m_shader->SetUniformValue("view", view);
	m_shader->SetUniformValue("projection", projection);
}

void LineDrawer::setOrigin(vec3 origin)
{
	if(m_lineVertics.size() >= 1)
		m_lineVertics[0] = origin;
	else
		m_lineVertics.push_back(origin);

	
	glBindBuffer(GL_ARRAY_BUFFER, m_buffers->m_vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3), &m_lineVertics[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void LineDrawer::setDestination(vec3 dest)
{
	if (m_lineVertics.size() >= 2)
		m_lineVertics[1] = dest;
	else
		m_lineVertics.push_back(dest);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffers->m_vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec3), sizeof(vec3), &m_lineVertics[1]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void LineDrawer::SetRay(const Ray & ray, float len)
{
	setOrigin(ray.getOrigin());
	setDestination(ray.GetPoint(len));
}

void LineDrawer::AddVertex(vec3 position)
{
	m_lineVertics.push_back(position);
	m_isChanged = true;
}
