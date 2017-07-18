#pragma once
/*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������MeshLoaderFactory
*	�衡������MeshLoader�Ĺ�����
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