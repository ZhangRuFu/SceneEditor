#pragma once
#include <GLM\glm.hpp>
#include "Entity.h"
/*
*	头文件说明:该头文件主要为TransformEditor的实现及其具体的TranslateAxis、RotateAxis、ScaleAxis实现
*
*/

/*
*	类名:TranslateSubAxis
*	描述：平移坐标轴的分量坐标轴（X、Y、Z轴）
*	最后修改时间：2017年7月31日21:57:10
*
*/
class TranslateAxis;

class TranslateSubAxis : public GameSpirit
{
private:
	TranslateAxis *m_axis;
	glm::vec3 m_direction;
	bool m_isHit = false;
	bool m_isActive = false;
	glm::ivec2 m_hitScreenPoint;


public:
	TranslateSubAxis(TranslateAxis *originAxis, Axis axis, vec3 color = vec3(1.0f, 1.0f, 1.0f));
	void SetAxisLen(float len);
	void Hit(void);
	void Active(void) { m_isActive = true; }
	void Invalid(void) { m_isActive = false; }
	virtual void Move(void);
};

/*
*	类名:TranslateAxis
*	描述：Transform中平移部分的可视化编辑器
*	最后修改时间：2017年7月31日21:57:10
*
*/

class TranslateAxis : public GameSpirit
{
private:
	Transform *m_targetTransform = nullptr;
	TranslateSubAxis *m_xAxis;
	TranslateSubAxis *m_yAxis;
	TranslateSubAxis *m_zAxis;
	float m_axisLen = 5.0f;

public:
	TranslateAxis(void);

	void setTargetTransform(Transform *transform);
	void ReleaseTarget(void);
	void TranslateTarget(vec3 t) { m_targetTransform->Move(t); }

	virtual void Move(void) {};
};