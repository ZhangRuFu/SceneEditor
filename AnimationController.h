#pragma once
#include "Model.h"
#include "AnimationModelDrawer.h"
#include "AnimationClipManager.h"
#include "ComponentManager.h"

class AnimationController : public Component
{
	//动画状态机信息
	SkeletonModel *m_model;
	AnimationModelDrawer *m_drawer;
	AnimationClipManager *m_clipManager;	//片段管理器
	AnimationClip *m_currentClip;			//当前动画片段
	AnimationClip *m_temporaryClip;			//临时动画片段

	BoneManager *m_boneManager;				//根骨骼
	vector<mat4> m_boneTransform;

	float m_spf;							//每帧所需时间
	float m_curTime;						//当前帧积累时间
	int m_keyCount;							//动画帧总数
	float m_curKey;							//当前帧
	bool m_isPlay;							//是否播放（更新帧）
	bool m_needUpdate;						//是否需要更新骨骼矩阵

	static map<string, AnimationController*> m_controllerMap;

public:
	AnimationController(AnimationModelDrawer *drawer, SkeletonModel *model);
	AnimationController(AnimationModelDrawer *drawer, SkeletonModel *model, string controllerName);
	void Pause(void) { m_needUpdate = m_isPlay = false; }
	void Play(void) { m_needUpdate = m_isPlay = true; }
	void PlayClip(AnimationClip *clip);
	void Move(void);
	void SetKey(float key) { m_curKey = key; m_needUpdate = true; }
	int GetLength(void) { return m_keyCount; }

	static AnimationController* GetController(string controllerName);

	virtual int GetComponentType(void) { return ComponentType::AnimationController; }

private:
	void Init(AnimationModelDrawer * drawer, SkeletonModel * model);
};