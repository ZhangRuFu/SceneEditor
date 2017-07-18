#pragma once
/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：MeshLoaderFactory
*	描　　述：MeshLoader的工厂类
*
*/

#include "MeshLoader.h"
#include "FBXMeshLoader.h"
#include "AssimpMeshLoader.h"

class MeshLoaderFactory
{
public:
	static MeshLoader* GetMeshLoader(string path);
};