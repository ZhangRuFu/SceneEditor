#include <GL\glew.h>
#include <GLM\gtx\euler_angles.hpp>
#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\type_ptr.hpp>
#include "Shader.h"
#include "RenderSystem.h"
#include "ResourceSystem.h"
#include "ColliderModelDrawer.h"

void ColliderModelDrawer::Draw()
{
	//Model¾ØÕó
	mat4 model;
	model = translate(model, m_collider->getWorldCenter());
	model = scale(model, vec3(m_collider->getRadius()));

	glUniformMatrix4fv(m_shader->GetUniformLocation("model"), 1, GL_FALSE, value_ptr(model));
	glUniform4fv(m_shader->GetUniformLocation("pureColor"), 1, value_ptr(vec4(GetColor(), 0.5)));

	int subMeshCount = m_mesh->GetMeshCount();
	for (int i = 0; i < subMeshCount; i++)
	{
		const StaticMesh *subMesh = m_mesh->GetMesh(i);

		glEnable(GL_BLEND);
		glBindVertexArray(m_buffers->m_vao[i]);
		glDrawElements(GL_TRIANGLES, subMesh->GetIndexCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glDisable(GL_BLEND);
	}
}

ColliderModelDrawer * ColliderModelDrawer::Create(Model * mesh, const SphereCollider * collider, vec3 color)
{
	ColliderModelDrawer *drawer = new ColliderModelDrawer(mesh, collider, color);
	drawer->LoadGraphicsBuffer(mesh);
	drawer->Register();
	return drawer;
}

