#pragma once
#pragma once
/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：MeshLoader
*	描　　述：模型加载器基类
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
