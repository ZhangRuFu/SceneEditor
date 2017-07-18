#pragma once
/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：AssimpMeshLoader
*	描　　述：利用Assimp模型库进行模型文件的模型加载器
*
*/

#include "MeshLoader.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <string>

#include "Mesh.h"

using std::string;

class AssimpMeshLoader : public MeshLoader
{
private:
	string m_meshDirectory;
	const aiScene *m_scene;
	vector<Texture*> m_textures;
	vector<StaticMesh*> m_subMeshes;

public:
	AssimpMeshLoader(string path);

public:

	Model* LoadModel();

private:
	void LoadMesh(aiNode *node);

	StaticMesh* MeshTransform(aiMesh *mesh);

	vector<Texture*> LoadTexture(aiMaterial *material, aiTextureType type, string id);
};
