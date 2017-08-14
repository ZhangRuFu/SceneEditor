#pragma once
#include <GLM\glm.hpp>
#include <GLM\gtc\quaternion.hpp>
#include <list>
#include "ComponentManager.h"
#include "CommonType.h"
/*
 *	����汾��Dragon Engine v0.1;
 *	�ࡡ������Transform
 *	�衡������GameEntity�Ļ��������������Ϸ�����λ�úͷ���
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
	vec3 m_position;			//λ��
	vec3 m_euler;				//ŷ����
	quat m_quaternion;			//��Ԫ��
	vec3 m_scale;				//����
	vec3 m_orientation;			//����
	mat4 m_modelMatrix;			//ģ�;���
	bool m_isUpdated = bool();	//ģ�;����Ƿ����

	Transform *m_father = nullptr;//������
	list<Transform*> m_children;//�������б�

	bool m_modelNormalCalc = true;		//����Model����ķ�ʽ

public:
	Transform(vec3 position = vec3(0, 0, 0), vec3 rotation = vec3(0, 0, 0) , vec3 scale = vec3(1, 1, 1));

public:
	void Move(float speed = 5);
	void Move(vec3 step);
	void ChangeOrientation(float angle);		//��ʱֻ����XZƽ����Y����ת
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
	
	mat4 GetModelMatrix(void);					//����Drawerͳһ�ӿ�
	void SetModelMatrixSpecialCalc(void) { m_modelNormalCalc = false; }
	void SetModelMatrixNormalCalc(void) { m_modelNormalCalc = true; }
	vec3 GetWorldOrigin(void);

	virtual int GetComponentType(void) { return ComponentType::Transform; }

private:
	void AttachFather(Transform *father) { m_father = father; }
	mat4 CalculateModelMatrix(void);
	mat4 CalculateSpecialModelMatrix(void);
};