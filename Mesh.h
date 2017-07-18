#pragma once
#include <glm\glm.hpp>
#include <string>
#include <sstream>
#include <vector>

using glm::vec2;
using glm::vec3;
using std::string;
using std::vector;
using std::stringstream;

//基础顶点
struct Vertex
{
	vec3 m_position;
	vec3 m_normal;
	vec2 m_texCoord;
};

//纹理
struct Texture
{
	unsigned int m_id;
	string m_type;
	string m_path;
};


/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：Mesh
*	描　　述：	网格类，对网格的抽象
				对“网格”概念的通用操作
*
*/
class Mesh
{
public:
	virtual int GetVertexCount(void) const = 0;
	virtual const Vertex* GetVertex(void) const = 0;
};

/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：StaticMesh
*	描　　述：静态模型的子网格
*
*/
class StaticMesh : public Mesh
{
private:
	vector<Vertex> *m_vertices;
	vector<Texture*> m_textures;
	vector<unsigned int> m_indices;

public:
	StaticMesh(vector<Vertex> *vertices, vector<Texture*> *textures, vector<unsigned int> *indices);

	virtual int GetVertexCount() const { return m_vertices->size(); }
	virtual const Vertex* GetVertex() const { return m_vertices->data(); }
	
	int GetIndexCount() const { return m_indices.size(); }
	void* GetIndex() const { return (void*)m_indices.data(); }
	
	int GetTextureCount() const { return m_textures.size(); }
	const Texture* GetTexture(int index) const { return m_textures[index]; }
	void AddTexture(Texture *tex) { m_textures.push_back(tex); }
};


/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：SkeletonMesh
*	描　　述：动态模型的子网格
*
*/


#define BONES_COUNT 4

//顶点权重
struct VertexWeight
{
	int m_boneIndex[BONES_COUNT];
	float m_weight[BONES_COUNT];

	VertexWeight(void);
	void AddVertexBoneInfo(int boneIndex, float weight);
};

//骨骼顶点（带权重信息）
struct SkeletonVertex : public Vertex
{
	VertexWeight m_weight;
};


class SkeletonMesh : public Mesh
{
	friend class FBXModelLoader;
private:
	vector<SkeletonVertex> m_vertices;
	vector<Texture*> m_textures;

public:
	SkeletonMesh(void){}

	int GetVertexCount() const { return m_vertices.size(); }
	virtual const SkeletonVertex* GetVertex() const { return m_vertices.data();}
	int GetTextureCount() const { return m_textures.size(); }
	const Texture* GetTexture(int index) const { return m_textures[index]; }
};