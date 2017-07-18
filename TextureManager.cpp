#include "TextureManager.h"
#include "RenderSystem.h"
#include <soil\SOIL.h>

#pragma comment(lib, "SOIL.lib")

TextureManager::TextureManager(void)
{

}
TextureManager * TextureManager::GetInstance(void)
{
	if (m_instance == nullptr)
		m_instance = new TextureManager();
	return m_instance;
}

unsigned int TextureManager::LoadTexture(string texPath, bool isAlpha)
{
	return m_instance->_LoadTexture(texPath, isAlpha);
}

unsigned int TextureManager::LoadCubeTexture(vector<const char*> texPaths)
{
	return m_instance->_LoadCubeTexture(texPaths);
}

unsigned int TextureManager::_LoadTexture(string texPath, bool isAlpha)
{
	int width, height;
	if (m_texMap.find(texPath) != m_texMap.end())
		return m_texMap[texPath];
	GLubyte* image = SOIL_load_image(texPath.c_str(), &width, &height, 0, isAlpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	unsigned int texID = RenderSystem::CreateTexture(TextureInfo(isAlpha, width, height, image));
	SOIL_free_image_data(image);
	m_texMap[texPath] = texID;
	return texID;
}

unsigned int TextureManager::_LoadCubeTexture(vector<const char*> texPaths)
{
	unsigned int cubeMapID;
	vector<TextureInfo> info;
	int width, height;
	for (unsigned i = 0; i < texPaths.size(); i++)
	{
		GLubyte *image = SOIL_load_image(texPaths[i], &width, &height, 0, SOIL_LOAD_RGB);
		info.push_back(TextureInfo(false, width, height, image));
	}
	cubeMapID = RenderSystem::CreateCubeTexture(&info);
	for (unsigned int i = 0; i < info.size(); i++)
		SOIL_free_image_data(info[i].getImageData());
	return cubeMapID;
}

TextureManager * TextureManager::m_instance = nullptr;