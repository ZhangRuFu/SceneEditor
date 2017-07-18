#pragma once
#pragma once
/*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������MeshLoader
*	�衡������ģ�ͼ���������
*
*/
#include <string>
#include "Model.h"

using std::string;

class MeshLoader
{
protected:
	string m_meshPath;

public:
	MeshLoader(string path);

	virtual Model* LoadModel() = 0;

};
