#pragma once
#include "Model.h"
#include "AnimationModelDrawer.h"
#include "AnimationClipManager.h"
#include "ComponentManager.h"

class AnimationController : public Component
{
	//����״̬����Ϣ
	SkeletonModel *m_model;
	AnimationModelDrawer *m_drawer;
	AnimationClipManager *m_clipManager;	//Ƭ�ι�����
	AnimationClip *m_currentClip;			//��ǰ����Ƭ��
	AnimationClip *m_temporaryClip;			//��ʱ����Ƭ��

	BoneManager *m_boneManager;				//������
	vector<mat4> m_boneTransform;

	float m_spf;							//ÿ֡����ʱ��
	float m_curTime;						//��ǰ֡����ʱ��
	int m_keyCount;							//����֡����
	float m_curKey;							//��ǰ֡
	bool m_isPlay;							//�Ƿ񲥷ţ�����֡��
	bool m_needUpdate;						//�Ƿ���Ҫ���¹�������

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