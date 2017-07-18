#pragma once
/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：TextureManager
*	描　　述：对引擎需要的材质进行加载和管理，确保同一份纹理只存在一个实例
*
*/
#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;

class TextureManager
{
private:
	static TextureManager *m_instance;

private:
	map<string, unsigned int> m_texMap;

private:
	TextureManager(void);

public:
	static TextureManager* GetInstance(void);
	static unsigned int LoadTexture(string texPath, bool isAlpha);
	static unsigned int LoadCubeTexture(vector<const char*> texPaths);

private:
	unsigned int _LoadTexture(string texPath, bool isAlpha);
	unsigned int _LoadCubeTexture(vector<const char*> texPaths);
	
};