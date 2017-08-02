#include "TransformEditor.h"
#include "LineRender.h"
#include "Model.h"
#include "InputSystem.h"
#include "ResourceSystem.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;

TranslateAxis::TranslateAxis(void)
{
	//各坐标轴对象
	m_xAxis = new TranslateSubAxis(this, Axis::X, vec3(1.0, 0.0, 0.0));
	m_yAxis = new TranslateSubAxis(this, Axis::Y, vec3(0.0, 1.0, 0.0));
	m_zAxis = new TranslateSubAxis(this, Axis::Z, vec3(0.0, 0.0, 1.0));
	
	m_transform->AttachChild(m_xAxis->GetTransform());
	m_transform->AttachChild(m_yAxis->GetTransform());
	m_transform->AttachChild(m_zAxis->GetTransform());

	m_xAxis->SetAxisLen(m_axisLen);
	m_yAxis->SetAxisLen(m_axisLen);
	m_zAxis->SetAxisLen(m_axisLen);

	//坐标轴线段
	LineDrawer *axisLine = LineDrawer::Create(m_transform);
	vec3 origin = vec3(0, 0, 0);
	axisLine->AddVertex(origin);
	axisLine->AddVertex(m_axisLen * vec3(1.0, 0.0, 0.0));
	axisLine->AddVertex(origin);
	axisLine->AddVertex(m_axisLen * vec3(0.0, 1.0, 0.0));
	axisLine->AddVertex(origin);
	axisLine->AddVertex(m_axisLen * vec3(0.0, 0.0, 1.0));
	AddComponent(*axisLine);
}

void TranslateAxis::setTargetTransform(Transform * transform)
{	
	m_targetTransform = transform;
	transform->AttachChild(GetTransform());
	m_xAxis->Active();
	m_yAxis->Active();
	m_zAxis->Active();
}

void TranslateAxis::ReleaseTarget(void)
{
	m_targetTransform = nullptr;
	m_xAxis->Invalid();
	m_yAxis->Invalid();
	m_zAxis->Invalid();
}


TranslateSubAxis::TranslateSubAxis(TranslateAxis *originAxis, Axis axis, vec3 color) : m_axis(originAxis)
{
	switch (axis)
	{
	case Axis::X:
		m_direction = vec3(1.0f, 0.0f, 0.0f);
		m_transform->Rotate(Axis::Z, -90.0f);
		break;
	case Axis::Y:
		m_direction = vec3(0.0f, 1.0f, 0.0f);
		break;
	case Axis::Z:
		m_direction = vec3(0.0f, 0.0f, 1.0f);
		m_transform->Rotate(Axis::X, 90.0f);
		break;
	}
	ModelArg arg(BasicMesh::CONE, color);
	AddComponent(arg);
}

void TranslateSubAxis::SetAxisLen(float len)
{
	m_transform->Move(len * m_direction);
}

void TranslateSubAxis::Hit(void)
{
	m_isHit = true;
	InputSystem::GetMousePosition(m_hitScreenPoint.x, m_hitScreenPoint.y);
}

void TranslateSubAxis::Move(void)
{
	if (!m_isActive)
		return;
	if (!m_isHit)
		return;
	if (InputSystem::isMouseKeyDown(MOUSE_KEY_LEFT))
	{
		vec3 origin;
		mat4 originModel = m_transform->getFatherTransform()->GetModelMatrix();
		origin = vec3(originModel * vec4(origin, 1.0f));
		vec3 axis = m_direction;
		axis = vec3(originModel * vec4(axis, 1.0f));
		vec2 so = ResourceSystem::GetMainCamera()->WorldToScreenPoint(origin);
		vec2 sa = ResourceSystem::GetMainCamera()->WorldToScreenPoint(axis);
		float slen = abs(length(sa - so));
		vec2 sd = glm::normalize(sa - so);

		ivec2 mousePoint;
		InputSystem::GetMousePosition(mousePoint.x, mousePoint.y);
		vec2 mouseVector = mousePoint - m_hitScreenPoint;

		//屏幕上的len要还原到世界空间中的len
		float len = glm::dot(mouseVector, sd);
		float wlen = 1 / slen * len;
		m_axis->TranslateTarget(wlen * m_direction);
		InputSystem::GetMousePosition(m_hitScreenPoint.x, m_hitScreenPoint.y);
	}
	else
		m_isHit = false;
}
