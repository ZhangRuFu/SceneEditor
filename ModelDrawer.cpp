#include <GL\glew.h>
#include <GLM\gtx\euler_angles.hpp>
#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\type_ptr.hpp>
#include "Shader.h"
#include "RenderSystem.h"
#include "ResourceSystem.h"
#include "ModelDrawer.h"

StaticModelDrawer::StaticModelDrawer(Model * mesh, string shaderName) : ModelDrawer(mesh, shaderName)
{
	m_mesh = dynamic_cast<StaticModel*>(mesh);
	if (m_mesh == nullptr)
		throw std::exception("StatcModelDrawer构造函数：mesh无法转换");
	m_buffers = nullptr;
}

void StaticModelDrawer::Draw()
{
	//Model矩阵
	mat4 model = m_transform->GetModelMatrix();
	/*model = translate(model, m_transform->GetPosition());
	vec3 rotation = m_transform->GetRotation();
	model = model * (mat4)eulerAngleXYZ(radians((double)rotation.x), radians((double)rotation.y), radians((double)rotation.z));
	model = scale(model, m_transform->GetScale());*/

	GLenum modelLocation = m_shader->GetUniformLocation("model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(model));

	int subMeshCount = m_mesh->GetMeshCount();
	int specularIndex = 0;
	int diffuseIndex = 0;
	stringstream ss;
	for (int i = 0; i < subMeshCount; i++)
	{
		const StaticMesh *subMesh = m_mesh->GetMesh(i);
		for (int j = 0; j < subMesh->GetTextureCount(); j++)
		{
			const Texture *texture = subMesh->GetTexture(j);
			ss.clear();
			glActiveTexture(GL_TEXTURE0 + j);
			string texName = texture->m_type;
			if (texName == "texSpecular")
				ss << specularIndex++;
			else if (texName == "texDiffuse")
				ss << diffuseIndex++;
			string str;
			ss >> str;
			texName = "material." + texName + str;
			GLenum location = glGetUniformLocation(m_shader->GetShaderID(), texName.c_str());
			glUniform1i(location, GL_TEXTURE0 + j);
			glBindTexture(GL_TEXTURE_2D, texture->m_id);
		}

		glBindVertexArray(m_buffers->m_vao[i]);
		glDrawElements(GL_TRIANGLES, subMesh->GetIndexCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		for (int i = 0; i < subMesh->GetTextureCount(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		glActiveTexture(GL_TEXTURE0);
	}
}

void StaticModelDrawer::PublicSet()
{
	//=====================================================这里和Camera都应该避免重复=================================================================
	Camera *camera = ResourceSystem::GetMainCamera();
	mat4 view = camera->GenViewMatrix();
	mat4 projection = camera->GenProjectionMatrix();
	vec3 viewerPosition = camera->GetViewPosition();
	Light *light = ResourceSystem::GetLight();
	vec3 lightColor = light->GetLightColor();
	vec3 lightPosition = light->GetTransform()->GetPosition();

	glUniform3fv(m_shader->GetUniformLocation("viewerPosition"), 1, value_ptr(viewerPosition));
	glUniform3fv(m_shader->GetUniformLocation("lightColor"), 1, value_ptr(lightColor));
	glUniform3fv(m_shader->GetUniformLocation("lightPosition"), 1, value_ptr(lightPosition));
	glUniform1f(m_shader->GetUniformLocation("material.shininess"), 32.0f);
	m_shader->SetUniformValue("view", view);
	m_shader->SetUniformValue("projection", projection);
}

StaticModelDrawer * StaticModelDrawer::Create(Model * mesh)
{
	StaticModelDrawer *drawer = new StaticModelDrawer(mesh);
	drawer->LoadGraphicsBuffer(mesh);
	return drawer;
}

GraphicsBuffer * StaticModelDrawer::LoadGraphicsBuffer(Model *model)
{
	m_buffers = GetModelBuffer(model);
	if (m_buffers != nullptr)
		return m_buffers;

	//产生新的Mesh缓冲区
	m_buffers = new GraphicsBuffer();
	int meshCount = model->GetMeshCount();
	m_buffers->m_vbo.resize(meshCount);
	m_buffers->m_vao.resize(meshCount);
	m_buffers->m_ebo.resize(meshCount);

	glGenBuffers(meshCount, m_buffers->m_vbo.data());
	glGenBuffers(meshCount, m_buffers->m_ebo.data());
	glGenVertexArrays(meshCount, m_buffers->m_vao.data());
	AddModelBuffer(model, m_buffers);

	for (int i = 0; i < meshCount; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_buffers->m_vbo[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers->m_ebo[i]);

		//数据传送
		const StaticMesh* subMesh = dynamic_cast<const StaticMesh*>(model->GetMesh(i));
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * subMesh->GetVertexCount(), subMesh->GetVertex(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * subMesh->GetIndexCount(), subMesh->GetIndex(), GL_STATIC_DRAW);

		glBindVertexArray(m_buffers->m_vao[i]);
		glBindBuffer(GL_ARRAY_BUFFER, m_buffers->m_vbo[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers->m_ebo[i]);
		
		//顶点属性指针
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, m_normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, m_texCoord));

		glBindVertexArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	return m_buffers;
}