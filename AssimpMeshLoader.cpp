#include "AssimpMeshLoader.h"
#include "TextureManager.h"
#include <iostream>

#pragma comment(lib, "assimp.lib")

using std::cout;
using std::endl;

AssimpMeshLoader::AssimpMeshLoader(string path) : MeshLoader(path)
{
	m_scene = nullptr;
}

Model* AssimpMeshLoader::LoadModel()
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(m_meshPath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if(!scene || !scene->mRootNode || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE)
	{
		cout << "加载模型Scene失败，错误信息:" << importer.GetErrorString() << endl;
		return nullptr;
	}
	//成功加载场景文件
	m_scene = scene;
	//模型文件所在目录
	m_meshDirectory = m_meshPath.substr(0, m_meshPath.find_last_of('\\') + 1);
	//预先扩充位置
	m_textures.reserve(m_scene->mNumTextures + 1);
	//加载节点
	LoadMesh(scene->mRootNode);
	return new StaticModel(m_subMeshes, m_textures);
}

void AssimpMeshLoader::LoadMesh(aiNode * node)
{
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = m_scene->mMeshes[node->mMeshes[i]];
		m_subMeshes.push_back(MeshTransform(mesh));
	}
	for (int i = 0; i < node->mNumChildren; i++)
		LoadMesh(node->mChildren[i]);
}

StaticMesh* AssimpMeshLoader::MeshTransform(aiMesh * mesh)
{
	vector<Vertex> *vertexes = new vector<Vertex>();	//顶点信息
	vector<unsigned int> indexes;						//索引信息
	vector<Texture*> textures;							//材质信息

	//网格顶点
	vertexes->resize(mesh->mNumVertices);
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		(*vertexes)[i].m_position.x = mesh->mVertices[i].x;
		(*vertexes)[i].m_position.y = mesh->mVertices[i].y;
		(*vertexes)[i].m_position.z = mesh->mVertices[i].z;

		(*vertexes)[i].m_normal.x = mesh->mNormals[i].x;
		(*vertexes)[i].m_normal.y = mesh->mNormals[i].y;
		(*vertexes)[i].m_normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0])
		{
			(*vertexes)[i].m_texCoord.x = mesh->mTextureCoords[0][i].x;
			(*vertexes)[i].m_texCoord.y = mesh->mTextureCoords[0][i].y;
		}
	}

	//网格索引
	indexes.reserve(mesh->mNumFaces * 3);
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++)
			indexes.push_back(face.mIndices[j]);
	}

	//网格纹理
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = m_scene->mMaterials[mesh->mMaterialIndex];
		vector<Texture*> diffuseTex = LoadTexture(material, aiTextureType::aiTextureType_DIFFUSE, "texDiffuse");
		textures.insert(textures.end(), diffuseTex.begin(), diffuseTex.end());
		vector<Texture*> specularTex = LoadTexture(material, aiTextureType::aiTextureType_SPECULAR, "texSpecular");
		textures.insert(textures.end(), specularTex.begin(), specularTex.end());
		vector<Texture*> refractTex = LoadTexture(material, aiTextureType::aiTextureType_AMBIENT, "texFraction");
		textures.insert(textures.end(), refractTex.begin(), refractTex.end());
	}

	return (new StaticMesh(vertexes, &textures, &indexes));
}

vector<Texture*> AssimpMeshLoader::LoadTexture(aiMaterial * material, aiTextureType type, string id)
{
	vector<Texture*> textures;
	for (int i = 0; i < material->GetTextureCount(type); i++)
	{
		bool isFind = false;
		aiString aiPath;
		material->GetTexture(type, i, &aiPath);
		string texPath(aiPath.C_Str());

		//缓存中是否存在纹理
		for (int j = 0; j < m_textures.size(); j++)
		{
			if (texPath == m_textures[j]->m_path)
			{
				textures.push_back(m_textures[j]);
				isFind = true;
				break;
			}
		}

		if (!isFind)
		{
			Texture *tex = new Texture();;
			tex->m_path = m_meshDirectory + texPath;
			tex->m_id = TextureManager::LoadTexture(tex->m_path, false);
			tex->m_type = id;
			m_textures.push_back(tex);
			textures.push_back(tex);
		}
	}

	return textures;
}
