#include <GL\glew.h>
#include <GLM\gtx\euler_angles.hpp>
#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\type_ptr.hpp>
#include "Shader.h"
#include "RenderSystem.h"
#include "ResourceSystem.h"
#include "SimpleModelDrawer.h"

void SimpleModelDrawer::Draw()
{
	//Model����
	mat4 model;
	model = translate(model, m_transform->getPosition());
	vec3 rotation = m_transform->getRotation();
	model = model * (mat4)eulerAngleXYZ(radians((double)rotation.x), radians((double)rotation.y), radians((double)rotation.z));
	model = scale(model, m_transform->getScale());

	glUniformMatrix4fv(m_shader->GetUniformLocation("model"), 1, GL_FALSE, value_ptr(model));
	glUniform4fv(m_shader->GetUniformLocation("pureColor"), 1, value_ptr(vec4(m_color, 1.0)));

	int subMeshCount = m_mesh->GetMeshCount();
	for (int i = 0; i < subMeshCount; i++)
	{
		const StaticMesh *subMesh = m_mesh->GetMesh(i);

		glBindVertexArray(m_buffers->m_vao[i]);
		glDrawElements(GL_TRIANGLES, subMesh->GetIndexCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}

void SimpleModelDrawer::PublicSet()
{
	//=====================================================�����Camera��Ӧ�ñ����ظ�=================================================================
	Camera *camera = ResourceSystem::GetMainCamera();
	mat4 view = camera->GenViewMatrix();
	mat4 projection = camera->GenProjectionMatrix();
	vec3 viewerPosition = camera->GetViewPosition();

	m_shader->SetUniformValue("view", view);
	m_shader->SetUniformValue("projection", projection);
}

SimpleModelDrawer * SimpleModelDrawer::Create(Model * mesh, Transform * transform, vec3 color)
{
	SimpleModelDrawer *drawer = new SimpleModelDrawer(mesh, transform, color);
	drawer->LoadGraphicsBuffer(mesh);
	drawer->Register();
	return drawer;
}

GraphicsBuffer * SimpleModelDrawer::LoadGraphicsBuffer(Model *mesh)
{
	m_buffers = GetModelBuffer(mesh);
	if (m_buffers != nullptr)
		return m_buffers;

	//�����µ�Mesh������
	m_buffers = new GraphicsBuffer();
	int subMeshCount = mesh->GetMeshCount();
	m_buffers->m_vbo.resize(subMeshCount);
	m_buffers->m_vao.resize(subMeshCount);
	m_buffers->m_ebo.resize(subMeshCount);

	glGenBuffers(subMeshCount, m_buffers->m_vbo.data());
	glGenBuffers(subMeshCount, m_buffers->m_ebo.data());
	glGenVertexArrays(subMeshCount, m_buffers->m_vao.data());
	AddModelBuffer(mesh, m_buffers);

	for (int i = 0; i < subMeshCount; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_buffers->m_vbo[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers->m_ebo[i]);

		//���ݴ���
		const StaticMesh* subMesh = dynamic_cast<const StaticMesh*>(mesh->GetMesh(i));
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * subMesh->GetVertexCount(), subMesh->GetVertex(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * subMesh->GetIndexCount(), subMesh->GetIndex(), GL_STATIC_DRAW);

		glBindVertexArray(m_buffers->m_vao[i]);
		glBindBuffer(GL_ARRAY_BUFFER, m_buffers->m_vbo[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers->m_ebo[i]);

		//��������ָ��
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

		glBindVertexArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	return m_buffers;
}