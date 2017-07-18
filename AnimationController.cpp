#include "AnimationController.h"
#include "ResourceSystem.h"
#include "Time.h"


AnimationController::AnimationController(AnimationModelDrawer * drawer, SkeletonModel * model)
{
	Init(drawer, model);
}

AnimationController::AnimationController(AnimationModelDrawer * drawer, SkeletonModel * model, string controllerName)
{
	Init(drawer, model);
	m_controllerMap[controllerName] = this;
}

void AnimationController::PlayClip(AnimationClip * clip)
{
	if (clip == nullptr)
		return;
	if (m_temporaryClip != nullptr)
		delete m_temporaryClip;
	m_currentClip = m_temporaryClip = clip;
	Play();
}

void AnimationController::Move(void)
{
	//每帧，计算骨骼Transform，交给Drawer
	if (m_isPlay && m_currentClip)
	{
		m_curTime += Time::GetDeltaTime();
		if (m_curTime < m_spf)
			return;
		m_curTime = 0;
		m_curKey++;
		if (m_curKey > m_currentClip->m_endKey)
			m_curKey = m_currentClip->m_startKey;
	}
	if (m_needUpdate)
	{
		m_boneManager->UpdateSkeleton(m_curKey, m_boneTransform);
		if (!m_isPlay)
			m_needUpdate = false;
	}
}

AnimationController * AnimationController::GetController(string controllerName)
{
	if (m_controllerMap.find(controllerName) != m_controllerMap.end())
		return m_controllerMap[controllerName];
}

void AnimationController::Init(AnimationModelDrawer * drawer, SkeletonModel * model)
{
	m_drawer = drawer;
	m_model = model;
	m_spf = 1.0 / 30 * 1000;		//ms
	m_curTime = 0;
	m_boneManager = m_model->GetBoneManager();
	m_boneTransform.resize(m_boneManager->GetBoneCount());
	m_drawer->UpdateBoneTransform(&m_boneTransform);
	m_curKey = 0;
	m_keyCount = model->GetAnimationKeyCount();
	m_isPlay = false;
	m_needUpdate = false;
	m_currentClip = m_temporaryClip = nullptr;
	ResourceSystem::Register(this);
}

map<string, AnimationController*> AnimationController::m_controllerMap;