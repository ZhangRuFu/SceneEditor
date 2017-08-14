#pragma once
#include <GLM\glm.hpp>
#include <string>
#include "Entity.h"
/*
*	头文件说明:该头文件主要为TransformEditor的实现及其具体的TransformEditor、RotateAxis、ScaleAxis实现
*
*/

class TransformEditor;

/*
*	类名:TransformAxis
*	描述：坐标轴基类
*	最后修改时间：2017年7月31日21:57:10
*
*/

class TransformAxis : public GameSpirit
{
protected:
	TransformEditor *m_axis;
	glm::vec3 m_direction;
	bool m_isHit = false;
	glm::ivec2 m_hitScreenPoint;


public:
	TransformAxis(TransformEditor *originAxis, Axis axis, const std::string name);
	void SetAxisLen(float len);
	void Hit(void);

protected:
	float ScreenToWorldLen(const glm::ivec2 &originScreenPoint, const glm::ivec2 &destScreenPoint, vec3 &direc);
};

/*
*
*	类名:TranslateAxis
*	描述：平移坐标轴的分量坐标轴（X、Y、Z轴）
*	最后修改时间：2017年7月31日21:57:10
*
*/

class TranslateAxis : public TransformAxis
{
public:
	TranslateAxis(TransformEditor *originAxis, Axis axis, vec3 color = vec3(1.0f, 1.0f, 1.0f));
	virtual void Move(void);
};

/*
*
*	类名:ScaleAxis
*	描述：缩放坐标轴的分量坐标轴（X、Y、Z轴）
*	最后修改时间：2017年8月4日18:20:26
*
*/

class ScaleAxis : public TransformAxis
{
public:
	ScaleAxis(TransformEditor *originAxis, Axis axis, vec3 color = vec3(1.0f, 1.0f, 1.0f));
	virtual void Move(void);
};

/*
*	类名:RotateAxis
*	描述：旋转坐标轴的分量坐标轴（X、Y、Z轴）
*	最后修改时间：2017年8月4日18:20:26
*
*/

class RotateAxis : public TransformAxis
{
public:
	RotateAxis(TransformEditor *originAxis, Axis axis);
	vec3 GetDirectionWorldPosition(void);
	virtual void Move(void);
};

/*
*
*	类名:TransformEditor
*	描述：Transform中平移部分的可视化编辑器
*	最后修改时间：2017年7月31日21:57:10
*
*/
class LineDrawer;
class SphereCollider;
class SimpleModelDrawer;
class TransformEditor : public GameSpirit
{
public:
	enum class TransformEditorType{None, Translate, Rotate, Scale};

private:
	Transform *m_targetTransform = nullptr;
	TranslateAxis *m_translateAxis = nullptr;
	ScaleAxis *m_scaleAxis = nullptr;
	RotateAxis *m_rotateAxis = nullptr;

	GameEntity *m_rotateEntity = nullptr;
	TransformAxis *m_curentAxis[3];
	LineDrawer *m_lineDrawer = nullptr;
	SimpleModelDrawer *m_rotateModel = nullptr;
	SphereCollider *m_rotateCollider = nullptr;
	
	
	float m_axisLen = 5.0f;		//轴默认长度
	TransformEditorType m_type = TransformEditorType::None;	//当前类型

public:
	TransformEditor(void);

	void SetTargetTransform(Transform *transform);
	void ReleaseTarget(void);
	void TranslateTarget(vec3 t) { m_targetTransform->Move(t); }
	void ScaleTarget(vec3 s) { m_targetTransform->Scale(s); }
	void RotateTarget(vec3 angle, bool isRadian) { m_targetTransform->Rotate(angle, isRadian); }
	void Hit(GameEntity &entity, vec3 hitPosition);
	void ChangeType(TransformEditorType type);

	virtual void Move(void);

private:
	void SetType(TransformEditorType type);
};