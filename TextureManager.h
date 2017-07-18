#pragma once
/*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������TextureManager
*	�衡��������������Ҫ�Ĳ��ʽ��м��غ͹���ȷ��ͬһ������ֻ����һ��ʵ��
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