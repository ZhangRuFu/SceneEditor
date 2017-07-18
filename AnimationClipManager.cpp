#include "AnimationClipManager.h"
#include "XSON.h"
#include <sstream>


AnimationClip * AnimationClipManager::GetAnimationClip(string clipName)
{
	for (vector<AnimationClip*>::const_iterator i = m_clips.cbegin(); i != m_clips.cend(); i++)
		if ((*i)->m_clipName == clipName)
			return (*i);
	return nullptr;
}

string AnimationClipManager::Serialize(void)
{
	string xson = "[Soldier]{";
	for (vector<AnimationClip*>::const_iterator i = m_clips.cbegin(); i != m_clips.cend(); i++)
	{
		xson.append((*i)->Serialize());
	}
	xson.append("}");
	return xson;
}

void AnimationClipManager::Deserialize(string path)
{
	//================================´ýÌí¼Ó==============================
}

string AnimationClip::Serialize(void)
{
	char str[50];
	sprintf_s<50>(str, "[Clip]{Name:%s;Start:%d;End:%d}", m_clipName.c_str(), m_startKey, m_endKey);
	string serialStr(str);
	return serialStr;
}

void AnimationClip::Deserialize(string path)
{
	using std::stringstream;
	stringstream ss;
	const map<string, string> *keyValue = XMLHelper::Import(path);
	ss << keyValue->at("Start");
	ss >> m_startKey;
	ss.clear();
	ss << keyValue->at("End");
	ss >> m_endKey;
	m_clipName = keyValue->at("Name");
}
