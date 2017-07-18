#pragma once
#include <string>
#include <vector>
#include "Serialize.h"

using std::string;
using std::vector;

struct AnimationClip : public ISerializable
{
	string m_clipName;
	float m_startKey;
	float m_endKey;

	AnimationClip(float startKey, float endKey, string clipName = "")
	{
		assert(endKey >= 0 && startKey >= 0 && endKey > startKey);
		m_startKey = startKey;
		m_endKey = endKey;
		m_clipName = clipName;
	}

	//@Override
	virtual string Serialize(void);
	virtual void Deserialize(string path);
};

class AnimationClipManager : public ISerializable
{
public:
	void AddClip(AnimationClip *clip) { m_clips.push_back(clip); };
	AnimationClip* GetAnimationClip(string clipName);
	int GetClipCount(void) { return m_clips.size(); }

	//@Override
	virtual string Serialize(void);
	virtual void Deserialize(string path);

private:
	vector<AnimationClip*> m_clips;
};