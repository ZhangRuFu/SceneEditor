#pragma once
#include <GLM\glm.hpp>
#include <list>
#include "ComponentManager.h"
/*
 *	����汾��Dragon Engine v0.1;
 *	�ࡡ������Transform
 *	�衡������GameEntity�Ļ��������������Ϸ�����λ�úͷ���
 *
 */

using glm::vec3;
using glm::mat4;
using std::list;

class Transform : public Component
{
	friend class GameEntity;
private:
	vec3 m_position;			//λ��
	vec3 m_rotation;			//��ת
	vec3 m_scale;				//����
	vec3 m_orientation;			//����
	mat4 m_modelMatrix;			//ģ�;���
	bool m_isUpdated = bool();	//ģ�;����Ƿ����

	Transform *m_father = nullptr;//������
	list<Transform*> m_children;//�������б�

public:
	Transform(vec3 position = vec3(0, 0, 0), vec3 rotation = vec3(0, 0, 0) , vec3 scale = vec3(1, 1, 1));

public:
	void Move(float speed = 5);
	void Move(vec3 step);
	void ChangeOrientation(float angle);		//��ʱֻ����XZƽ����Y����ת
	void Scale(vec3 &scale);
	
	vec3 getPosition(void) const;
	vec3 getRotation(void) const;
	vec3 getScale(void) const;
	vec3 getOrientation(void) const;

	void ReadyRender() { m_isUpdated = false; }
	void AttachChild(Transform *child);
	int GetChildCount(void) { return m_children.size(); }
	mat4 GetModelMatrix(void);

	virtual int GetComponentType(void) { return ComponentType::Transform; }

private:
	void AttachFather(Transform *father) { m_father = father; }
};