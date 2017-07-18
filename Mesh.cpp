#include "Mesh.h"

StaticMesh::StaticMesh(vector<Vertex> *vertices, vector<Texture*>* textures, vector<unsigned int>* indices) :
	m_vertices(vertices), m_textures(*textures), m_indices(*indices)
{

}

//===================================================VertexWeight====================================================
VertexWeight::VertexWeight(void)
{
	for (int i = 0; i < BONES_COUNT; i++)
	{
		m_boneIndex[i] = 0;
		m_weight[i] = 0;
	}
}

void VertexWeight::AddVertexBoneInfo(int boneIndex, float weight)
{
	for (int i = 0; i < BONES_COUNT; i++)
	{
		if (m_weight[i] != 0)
			continue;
		m_boneIndex[i] = boneIndex;
		m_weight[i] = weight;
		break;
	}
}