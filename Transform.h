#pragma once
#include <GLM\glm.hpp>
#include <GLM\gtc\quaternion.hpp>
#include <list>
#include "ComponentManager.h"
#include "CommonType.h"
/*
 *	引擎版本：Dragon Engine v0.1;
 *	类　　名：Transform
 *	描　　述：GameEntity的基本组件，代表游戏对象的位置和方向
 *
 */

using glm::vec3;
using glm::quat;
using glm::mat4;
using std::list;

class Transform : public Component
{
	friend class GameEntity;
private:
	vec3 m_position;			//位置
	vec3 m_euler;				//欧拉角
	quat m_quaternion;			//四元数
	vec3 m_scale;				//缩放
	vec3 m_orientation;			//方向
	mat4 m_modelMatrix;			//模型矩阵
	bool m_isUpdated = bool();	//模型矩阵是否更新

	Transform *m_father = nullptr;//父物体
	list<Transform*> m_children;//子物体列表

	bool m_modelNormalCalc = true;		//计算Model矩阵的方式

public:
	Transform(vec3 position = vec3(0, 0, 0), vec3 rotation = vec3(0, 0, 0) , vec3 scale = vec3(1, 1, 1));

public:
	void Move(float speed = 5);
	void Move(vec3 step);
	void ChangeOrientation(float angle);		//暂时只能在XZ平面绕Y轴旋转
	void Scale(vec3 &scale);
	void Rotate(Axis axis, float angle, bool isLocal = true);
	void Rotate(vec3 angle, bool isRadian = false);
	
	vec3 GetPosition(void) const;
	void SetPosition(vec3 position) { m_position = position; }
	vec3 GetRotation(void) const;
	vec3 GetScale(void) const;
	void SetScale(vec3 scale) { m_scale = scale; }
	vec3 GetOrientation(void) const;
	Transform* getFatherTransform(void) const { return m_father; }

	void ReadyRender() { m_isUpdated = false; }
	void AttachChild(Transform *child);
	int GetChildCount(void) { return m_children.size(); }
	
	mat4 GetModelMatrix(void);					//对外Drawer统一接口
	void SetModelMatrixSpecialCalc(void) { m_modelNormalCalc = false; }
	void SetModelMatrixNormalCalc(void) { m_modelNormalCalc = true; }
	vec3 GetWorldOrigin(void);

	virtual int GetComponentType(void) { return ComponentType::Transform; }

private:
	void AttachFather(Transform *father) { m_father = father; }
	mat4 CalculateModelMatrix(void);
	mat4 CalculateSpecialModelMatrix(void);
};