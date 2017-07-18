#include "Collider.h"
#include "Model.h"

void SphereCollider::CalcModelBoundingBox(Model * model)
{
	int sumCount = 0;
	vec3 sumPosition;
	int meshCount = model->GetMeshCount();
	for (int i = 0; i < meshCount; ++i)
	{
		const Mesh *mesh = model->GetMesh(i);
		int vCount = mesh->GetVertexCount();
		sumCount += vCount;
		const Vertex *vertex = mesh->GetVertex();
		for (int j = 0; j < vCount; j++)
			sumPosition += vertex[j].m_position;
	}
	sumPosition /= sumCount;
	m_center = sumPosition;
	vec3 len;
	float length = m_radius = 0;
	for (int i = 0; i < meshCount; ++i)
	{
		const Mesh *mesh = model->GetMesh(i);
		int vCount = mesh->GetVertexCount();
		const Vertex *vertex = mesh->GetVertex();
		for (int j = 0; j < vCount; j++)
		{
			len = vertex[j].m_position - m_center;
			length = glm::dot(len, len);
			if (length > m_radius)
				m_radius = length;
		}
	}
	m_radius = sqrt(m_radius);
}
