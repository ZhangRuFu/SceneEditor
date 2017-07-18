#include "MeshLoaderFactory.h"

MeshLoader * MeshLoaderFactory::GetMeshLoader(string path)
{
	string suffix = path.substr(path.find_last_of('.'));
	if (suffix == ".fbx" || suffix == ".FBX")
		return (new FBXModelLoader(path));
	else
		return (new AssimpMeshLoader(path));
}
