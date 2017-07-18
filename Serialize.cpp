#include "Serialize.h"
#include <queue>

using std::queue;

void XMLHelper::Export(string path, ISerializable *object)
{
	TiXmlDocument *doc = new TiXmlDocument();
	TiXmlElement *root = Parse(object);
	doc->LinkEndChild(root);
	doc->SaveFile(path.c_str());
	doc->Clear();
	delete doc;
}

const map<string, string>* XMLHelper::Import(string path)
{
	map<string, string> *keyValue = new map<string, string>();
	TiXmlDocument *doc = new TiXmlDocument();
	doc->LoadFile(path.data());
	TiXmlElement *root = doc->RootElement();
	queue<TiXmlElement*> xmlQueue;
	xmlQueue.push(root);
	while (!xmlQueue.empty())
	{
		TiXmlElement *curElement = xmlQueue.front();
		const char *text = curElement->GetText();
		if (text != nullptr)
			(*keyValue)[curElement->Value()] = text;
		TiXmlElement * child = curElement->FirstChildElement();
		while (child != nullptr)
		{
			xmlQueue.push(child);
			child = child->NextSiblingElement();
		}
		xmlQueue.pop();
	}
	doc->Clear();
	delete doc;
	return keyValue;
}

TiXmlElement * XMLHelper::Parse(ISerializable *object)
{
	string src = object->Serialize();
	string rootName = src.substr(1, src.find(']') - 1);
	TiXmlElement *root = new TiXmlElement(rootName.data());
	int preIndex = 0;
	int findIndex = 0;
	preIndex = findIndex = src.find('{', 0) + 1;
	while (findIndex < src.size())
	{
		findIndex = src.find('-', findIndex);
		if (findIndex == string::npos)
			findIndex = src.size() - 1;
		string item = src.substr(preIndex, findIndex - preIndex);
		int colonIndex = item.find(':');
		string key = item.substr(0, colonIndex);
		string value = item.substr(colonIndex + 1);
		TiXmlElement *element = new TiXmlElement(key.data());
		element->LinkEndChild(new TiXmlText(value.data()));
		root->LinkEndChild(element);
		preIndex = ++findIndex;
	}
	return root;
}
