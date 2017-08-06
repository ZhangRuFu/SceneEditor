#include "TransformEditor.h"
#include "LineRender.h"
#include "Model.h"
#include "InputSystem.h"
#include "ResourceSystem.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using std::string;

TransformEditor::TransformEditor(void) : GameSpirit("TransformEditor", "TransformEditor")
{
	//各坐标轴对象
	m_translateAxis = new TranslateAxis[3]{ TranslateAxis(this, Axis::X, vec3(1.0, 0.0, 0.0)), TranslateAxis(this, Axis::Y, vec3(0.0, 1.0, 0.0)), TranslateAxis(this, Axis::Z, vec3(0.0, 0.0, 1.0))};
	m_scaleAxis = new ScaleAxis[3]{ ScaleAxis(this, Axis::X, vec3(1.0, 0.0, 0.0)), ScaleAxis(this, Axis::Y, vec3(0.0, 1.0, 0.0)), ScaleAxis(this, Axis::Z, vec3(0.0, 0.0, 1.0)) };
	
	for (int i = 0; i < 3; ++i)
	{
		m_translateAxis[i].Disable();
		m_scaleAxis[i].Disable();
		m_transform->AttachChild(m_translateAxis[i].GetTransform());
		m_transform->AttachChild(m_scaleAxis[i].GetTransform());
		m_translateAxis[i].SetAxisLen(m_axisLen);
		m_scaleAxis[i].SetAxisLen(m_axisLen);
	}

	//坐标轴线段
	LineDrawer *axisLine = new LineDrawer();
	vec3 origin = vec3(0, 0, 0);
	axisLine->AddVertex(origin);
	axisLine->AddVertex(m_axisLen * vec3(1.0, 0.0, 0.0));
	axisLine->AddVertex(origin);
	axisLine->AddVertex(m_axisLen * vec3(0.0, 1.0, 0.0));
	axisLine->AddVertex(origin);
	axisLine->AddVertex(m_axisLen * vec3(0.0, 0.0, 1.0));
	AddComponent(*axisLine);

	ChangeType(TransformEditorType::Translate);
	ChangeType(TransformEditorType::Scale);
	m_transform->ChangeModelMatrixCalcType(Transform::ModelMatrixType::Translate | Transform::ModelMatrixType::Rotation | Transform::ModelMatrixType::EXCEPTTOP);
}

void TransformEditor::SetTargetTransform(Transform * transform)
{	
	m_targetTransform = transform;
	transform->AttachChild(GetTransform());
	Enable();
	for (int i = 0; i < 3; ++i)
		m_curentAxis[i].Enable();
}

void TransformEditor::ReleaseTarget(void)
{
	Disable();
	m_targetTransform = nullptr;
	for (int i = 0; i < 3; ++i)
		m_curentAxis[i].Disable();
}

void TransformEditor::Hit(GameEntity &axis)
{
	TransformAxis *transformAxis = dynamic_cast<TransformAxis*>(&axis);
	if (transformAxis)
		transformAxis->Hit();
}

void TransformEditor::ChangeType(TransformEditorType type)
{
	m_type = type;
	switch (type)
	{
	case TransformEditor::TransformEditorType::Translate:
		m_curentAxis = m_translateAxis;
		break;
	case TransformEditor::TransformEditorType::Rotate:
		//===========================================================================================================================
		break;
	case TransformEditor::TransformEditorType::Scale:
		m_curentAxis = m_scaleAxis;
		break;
	}
}



TranslateAxis::TranslateAxis(TransformEditor *originAxis, Axis axis, vec3 color) : TransformAxis(originAxis, axis, "TranslateAxis")
{
	switch (axis)
	{
	case Axis::X:
		m_transform->Rotate(Axis::Z, -90.0f);
		break;
	case Axis::Z:
		m_transform->Rotate(Axis::X, 90.0f);
		break;
	}
	ModelArg arg(*this, BasicMesh::CONE, color);
	AddComponent(arg);
	m_transform->ChangeModelMatrixCalcType(Transform::ModelMatrixType::Translate | Transform::ModelMatrixType::EXCEPTTOP);
}


void TranslateAxis::Move(void)
{
	if (!m_isHit)
		return;
	if (InputSystem::isMouseKeyDown(MOUSE_KEY_LEFT))
	{
		ivec2 mousePoint;
		InputSystem::GetMousePosition(mousePoint.x, mousePoint.y);

		float wlen = ScreenToWorldLen(m_hitScreenPoint, mousePoint);
		m_axis->TranslateTarget(wlen * m_direction);
		InputSystem::GetMousePosition(m_hitScreenPoint.x, m_hitScreenPoint.y);
	}
	else
		m_isHit = false;
}


ScaleAxis::ScaleAxis(TransformEditor * originAxis, Axis axis, vec3 color) : TransformAxis(originAxis, axis, "ScaleAxis")
{
	ModelArg arg(*this, BasicMesh::CUBE, color);
	m_transform->ChangeModelMatrixCalcType(Transform::ModelMatrixType::Translate | Transform::ModelMatrixType::EXCEPTTOP);
	m_transform->SetScale(vec3(0.3));
	AddComponent(arg);
}

void ScaleAxis::Move(void)
{
	if (!m_isHit)
		return;
	if (InputSystem::isMouseKeyDown(MOUSE_KEY_LEFT))
	{
		ivec2 mousePoint;
		InputSystem::GetMousePosition(mousePoint.x, mousePoint.y);

		float wlen = ScreenToWorldLen(m_hitScreenPoint, mousePoint);
		m_axis->ScaleTarget(wlen * m_direction);
		InputSystem::GetMousePosition(m_hitScreenPoint.x, m_hitScreenPoint.y);
	}
	else
		m_isHit = false;
}



TransformAxis::TransformAxis(TransformEditor * originAxis, Axis axis, const string name) : m_axis(originAxis), GameSpirit(name, "TransformEditor")
{
	switch (axis)
	{
	case Axis::X:
		m_direction = vec3(1.0f, 0.0f, 0.0f);
		break;
	case Axis::Y:
		m_direction = vec3(0.0f, 1.0f, 0.0f);
		break;
	case Axis::Z:
		m_direction = vec3(0.0f, 0.0f, 1.0f);
		break;
	}
}

void TransformAxis::SetAxisLen(float len)
{
	m_transform->Move(len * m_direction);
}

void TransformAxis::Hit(void)
{
	m_isHit = true;
	InputSystem::GetMousePosition(m_hitScreenPoint.x, m_hitScreenPoint.y);
}

float TransformAxis::ScreenToWorldLen(const ivec2 &originScreenPoint, const ivec2 &destScreenPoint)
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

	vec2 mouseVector = destScreenPoint - originScreenPoint;

	//屏幕上的len要还原到世界空间中的len
	float len = glm::dot(mouseVector, sd);
	float wlen = 1 / slen * len;
	return wlen;
}
