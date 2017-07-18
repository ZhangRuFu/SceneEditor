#pragma once
#include <string>
#include <map>
#include <TinyXML\tinyxml.h>

using std::string;
using std::map;


class ISerializable
{
public:
	virtual string Serialize(void) = 0;
	virtual void Deserialize(string path) = 0;
};