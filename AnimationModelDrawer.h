#pragma once
/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：AnimationModelDrawer
*	描　　述：继承自Drawer，适用于骨骼动画的渲染器
*
*/
#include "Drawer.h"
#include "Transform.h"
#include <map>

class SkeletonModel;
class Model;

using std::map;

class AnimationModelDrawer : public ModelDrawer
{
private:
	SkeletonModel *m_model;
	Transform *m_transform;
	vector<mat4> *m_boneTransform;

protected:
	AnimationModelDrawer(Model *model, Transform *transform, string shaderName = "AnimationModel");

public:
	void UpdateBoneTransform(vector<mat4> *boneTransform) { m_boneTransform = boneTransform; }

	virtual void Draw(void);
	virtual void PublicSet(void);
	virtual int GetComponentType(void) { return (int)ComponentType::Drawer::AnimationModelDrawer; }
	virtual GraphicsBuffer* LoadGraphicsBuffer(Model *model);

	static AnimationModelDrawer* Create(Model *model, Transform *transform);
};