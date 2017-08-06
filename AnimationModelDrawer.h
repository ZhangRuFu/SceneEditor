#pragma once
/*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������AnimationModelDrawer
*	�衡�������̳���Drawer�������ڹ�����������Ⱦ��
*
*/
#include <map>
#include <GLM\glm.hpp>
#include "Drawer.h"

class SkeletonModel;
class Model;

using std::map;

class AnimationModelDrawer : public ModelDrawer
{
private:
	SkeletonModel *m_model;
	vector<glm::mat4> *m_boneTransform;

protected:
	AnimationModelDrawer(Model *model, string shaderName = "AnimationModel");

public:
	void UpdateBoneTransform(vector<glm::mat4> *boneTransform) { m_boneTransform = boneTransform; }

	virtual void Draw(void);
	virtual void PublicSet(void);
	virtual int GetComponentType(void) { return (int)ComponentType::Drawer::AnimationModelDrawer; }
	virtual GraphicsBuffer* LoadGraphicsBuffer(Model *model);

	static AnimationModelDrawer* Create(Model *model);
};