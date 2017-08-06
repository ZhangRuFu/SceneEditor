#pragma once
#include <GLM\glm.hpp>
#include <string>
#include "Entity.h"
/*
*	ͷ�ļ�˵��:��ͷ�ļ���ҪΪTransformEditor��ʵ�ּ�������TransformEditor��RotateAxis��ScaleAxisʵ��
*
*/

class TransformEditor;

/*
*	����:TransformAxis
*	���������������
*	����޸�ʱ�䣺2017��7��31��21:57:10
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
	float ScreenToWorldLen(const glm::ivec2 &originScreenPoint, const glm::ivec2 &destScreenPoint);
};

/*
*	����:TranslateAxis
*	������ƽ��������ķ��������ᣨX��Y��Z�ᣩ
*	����޸�ʱ�䣺2017��7��31��21:57:10
*
*/

class TranslateAxis : public TransformAxis
{
public:
	TranslateAxis(TransformEditor *originAxis, Axis axis, vec3 color = vec3(1.0f, 1.0f, 1.0f));
	virtual void Move(void);
};

/*
*	����:ScaleAxis
*	����������������ķ��������ᣨX��Y��Z�ᣩ
*	����޸�ʱ�䣺2017��8��4��18:20:26
*
*/

class ScaleAxis : public TransformAxis
{
public:
	ScaleAxis(TransformEditor *originAxis, Axis axis, vec3 color = vec3(1.0f, 1.0f, 1.0f));
	virtual void Move(void);
};


/*
*
*	����:TransformEditor
*	������Transform��ƽ�Ʋ��ֵĿ��ӻ��༭��
*	����޸�ʱ�䣺2017��7��31��21:57:10
*
*/

class TransformEditor : public GameSpirit
{
public:
	enum class TransformEditorType{Translate, Rotate, Scale};

private:
	Transform *m_targetTransform = nullptr;
	TranslateAxis *m_translateAxis = nullptr;
	ScaleAxis *m_scaleAxis = nullptr;

	TransformAxis *m_curentAxis = nullptr;
	
	
	float m_axisLen = 5.0f;		//��Ĭ�ϳ���
	TransformEditorType m_type = TransformEditorType::Translate;	//��ǰ����

public:
	TransformEditor(void);

	void SetTargetTransform(Transform *transform);
	void ReleaseTarget(void);
	void TranslateTarget(vec3 t) { m_targetTransform->Move(t); }
	void ScaleTarget(vec3 s) { m_targetTransform->Scale(s); }
	void Hit(GameEntity &axis);
	void ChangeType(TransformEditorType type);

	virtual void Move(void) {};
};