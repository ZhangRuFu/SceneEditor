#include "TransformEditor.h"
#include "LineRender.h"
#include "Model.h"
#include "InputSystem.h"
#include "ResourceSystem.h"
#include "RenderMode.h"
#include "SimpleModelDrawer.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using std::string;

TransformEditor::TransformEditor(void) : GameSpirit("TransformEditor", "TransformEditor")
{
	//各坐标轴对象
	m_translateAxis = new TranslateAxis[3]{ TranslateAxis(this, Axis::X, vec3(1.0, 0.0, 0.0)), TranslateAxis(this, Axis::Y, vec3(0.0, 1.0, 0.0)), TranslateAxis(this, Axis::Z, vec3(0.0, 0.0, 1.0))};
	m_scaleAxis = new ScaleAxis[3]{ ScaleAxis(this, Axis::X, vec3(1.0, 0.0, 0.0)), ScaleAxis(this, Axis::Y, vec3(0.0, 1.0, 0.0)), ScaleAxis(this, Axis::Z, vec3(0.0, 0.0, 1.0)) };
	m_rotateAxis = new RotateAxis[3]{RotateAxis(this, Axis::X), RotateAxis(this, Axis::Y), RotateAxis(this, Axis::Z) };

	for (int i = 0; i < 3; ++i)
	{
		m_translateAxis[i].Disable();
		m_scaleAxis[i].Disable();
		m_rotateAxis[i].Disable();

		m_transform->AttachChild(m_translateAxis[i].GetTransform());
		m_transform->AttachChild(m_scaleAxis[i].GetTransform());
		m_transform->AttachChild(m_rotateAxis[i].GetTransform());
		
		m_translateAxis[i].SetAxisLen(m_axisLen);
		m_scaleAxis[i].SetAxisLen(m_axisLen);
		m_rotateAxis[i].SetAxisLen(0);
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
	m_lineDrawer = axisLine;

	//为旋转轴的碰撞体组件
	m_rotateCollider = new SphereCollider();
	m_rotateCollider->SetRadius(2.0f);
	AddComponent(*m_rotateCollider);

	m_rotateEntity = new GameEntity();
	m_transform->AttachChild(m_rotateEntity->GetTransform());
	ModelArg arg(*m_rotateEntity, BasicMesh::SPHERE, vec4(1.0f ,1.0f ,1.0f, 0.0f));
	m_rotateEntity->AddComponent(arg);
	m_rotateEntity->GetTransform()->SetScale(vec3(1.99f));
	m_rotateModel = dynamic_cast<SimpleModelDrawer*>(m_rotateEntity->GetComponent((int)ComponentType::Drawer::SimpleModelDrawer));
	m_rotateModel->AddRenderMode(RenderModeType::DepthTest, DepthTestRenderMode::AlwaysPass);
	m_rotateModel->AddRenderMode(RenderModeType::Blend, 0);
	m_rotateEntity->Disable();

	SetType(TransformEditorType::Translate);
	m_transform->SetModelMatrixSpecialCalc();
}

void TransformEditor::SetTargetTransform(Transform * transform)
{	
	m_targetTransform = transform;
	transform->AttachChild(GetTransform());
	Enable();
	for (int i = 0; i < 3; ++i)
		m_curentAxis[i]->Enable();

	if (m_type == TransformEditorType::Rotate)
	{
		m_rotateEntity->Enable();

	}
}

void TransformEditor::ReleaseTarget(void)
{
	Disable();
	m_targetTransform = nullptr;
	for (int i = 0; i < 3; ++i)
		m_curentAxis[i]->Disable();
}

void TransformEditor::Hit(GameEntity &entity, vec3 hitPosition)
{
	if (m_type != TransformEditor::TransformEditorType::Rotate)
	{
		if (TransformAxis *transformAxis = dynamic_cast<TransformAxis*>(&entity))
			transformAxis->Hit();
	}

	//判断旋转坐标轴
	RotateAxis *minAxis = m_rotateAxis;
	float minDistance = 0;
	for (int i = 0; i < 3; ++i)
	{
		vec3 direc = m_rotateAxis[i].GetDirectionWorldPosition();
		float distance = abs(glm::dot(hitPosition - m_rotateAxis[i].GetTransform()->GetWorldOrigin(), m_rotateAxis[i].GetDirectionWorldPosition()));
		if (i == 0)
			minDistance = distance;
		else
		{
			if (distance < minDistance)
			{
				minDistance = distance;
				minAxis = m_rotateAxis + i;
			}
		}
	}
	minAxis->Hit();
}

void TransformEditor::ChangeType(TransformEditorType type)
{
	if (m_type == type)
		return;
	m_type = type;
	for (int i = 0; i < 3; ++i)
		m_curentAxis[i]->Disable();
	switch (type)
	{
	case TransformEditor::TransformEditorType::Translate:
		for (int i = 0; i < 3; ++i)
			m_curentAxis[i] = m_translateAxis + i;
		m_lineDrawer->Enable();
		m_rotateCollider->Disable();
		break;
	case TransformEditor::TransformEditorType::Rotate:
		for (int i = 0; i < 3; ++i)
			m_curentAxis[i] = m_rotateAxis + i;
		m_lineDrawer->Disable();
		m_rotateCollider->Enable();
		break;
	case TransformEditor::TransformEditorType::Scale:
		for (int i = 0; i < 3; ++i)
			m_curentAxis[i] = m_scaleAxis + i;
		m_lineDrawer->Enable();
		m_rotateCollider->Disable();
		break;
	}
	for (int i = 0; i < 3; ++i)
		m_curentAxis[i]->Enable();
}

void TransformEditor::Move(void)
{
	if (InputSystem::isKeyDown(KEY_F1))
		ChangeType(TransformEditorType::Rotate);
	else if (InputSystem::isKeyDown(KEY_F2))
		ChangeType(TransformEditorType::Translate);
	else if (InputSystem::isKeyDown(KEY_F4))
		ChangeType(TransformEditorType::Scale);
}

void TransformEditor::SetType(TransformEditorType type)
{
	m_type = type;
	switch (type)
	{
	case TransformEditor::TransformEditorType::Translate:
		for (int i = 0; i < 3; ++i)
			m_curentAxis[i] = m_translateAxis + i;
		m_lineDrawer->Enable();
		m_rotateCollider->Disable();
		break;
	case TransformEditor::TransformEditorType::Rotate:
		for (int i = 0; i < 3; ++i)
			m_curentAxis[i] = m_rotateAxis + i;
		m_lineDrawer->Disable();
		m_rotateCollider->Enable();
		break;
	case TransformEditor::TransformEditorType::Scale:
		for (int i = 0; i < 3; ++i)
			m_curentAxis[i] = m_scaleAxis + i;
		m_lineDrawer->Enable();
		m_rotateCollider->Disable();
		break;
	}
}



TranslateAxis::TranslateAxis(TransformEditor *originAxis, Axis axis, vec3 color) : TransformAxis(originAxis, axis, "TranslateAxis")
{
	switch (axis)
	{
	case Axis::X:
		m_transform->Rotate(Axis::Z, -90.0f, false);
		break;
	case Axis::Z:
		m_transform->Rotate(Axis::X, 90.0f, false);
		break;
	}
	ModelArg arg(*this, BasicMesh::CONE, color);
	AddComponent(arg);
	m_transform->SetModelMatrixSpecialCalc();
}


void TranslateAxis::Move(void)
{
	if (!m_isHit)
		return;
	if (InputSystem::isMouseKeyDown(MOUSE_KEY_LEFT))
	{
		ivec2 mousePoint;
		InputSystem::GetMousePosition(mousePoint.x, mousePoint.y);

		float wlen = ScreenToWorldLen(m_hitScreenPoint, mousePoint, m_direction);
		m_axis->TranslateTarget(wlen * m_direction);
		InputSystem::GetMousePosition(m_hitScreenPoint.x, m_hitScreenPoint.y);
	}
	else
		m_isHit = false;
}


ScaleAxis::ScaleAxis(TransformEditor * originAxis, Axis axis, vec3 color) : TransformAxis(originAxis, axis, "ScaleAxis")
{
	ModelArg arg(*this, BasicMesh::CUBE, color);
	m_transform->SetModelMatrixSpecialCalc();
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

		float wlen = ScreenToWorldLen(m_hitScreenPoint, mousePoint, m_direction);
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

float TransformAxis::ScreenToWorldLen(const ivec2 &originScreenPoint, const ivec2 &destScreenPoint, vec3 &direc)
{
	vec3 origin;
	mat4 originModel = m_transform->getFatherTransform()->GetModelMatrix();
	origin = vec3(originModel * vec4(origin, 1.0f));
	vec3 axis = direc;
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

RotateAxis::RotateAxis(TransformEditor * originAxis, Axis axis) : TransformAxis(originAxis, axis, "RotateAxis")
{
	switch (axis)
	{
	case Axis::X:
		m_transform->Rotate(Axis::Z, -90.0f, false);
		break;
	case Axis::Z:
		m_transform->Rotate(Axis::X, 90.0f, false);
		break;
	}
	m_transform->SetScale(vec3(2.0f, 2.0f, 2.0f));
	LineDrawer *ld = new LineDrawer(LineDrawer::BasicLine::Ring);
	ld->ChangeRenderLevel(RenderLevel::AfterEntity);
	//ld->AddRenderMode(RenderModeType::DepthTest, DepthTestRenderMode::DepthTestMode::AlwaysPass);
	ld->SetColor(m_direction);
	AddComponent(*ld);
}

vec3 RotateAxis::GetDirectionWorldPosition(void)
{
	vec3 origin = m_transform->GetWorldOrigin();
	vec3 direc = m_transform->GetModelMatrix() * vec4(0.0, 1.0, 0.0, 1.0);
	return glm::normalize(direc - origin);
}

void RotateAxis::Move(void)
{
	if (!m_isHit)
		return;
	if (InputSystem::isMouseKeyDown(MOUSE_KEY_LEFT))
	{
		ivec2 mousePoint;
		InputSystem::GetMousePosition(mousePoint.x, mousePoint.y);
		if (mousePoint == m_hitScreenPoint)
			return;

		vec3 origin;
		mat4 originModel = m_transform->GetModelMatrix();
		origin = vec3(originModel * vec4(origin, 1.0f));
		vec3 axis = vec3(originModel * vec4(0.0f, 1.0f, 0.0f, 1.0f));

		vec2 so = ResourceSystem::GetMainCamera()->WorldToScreenPoint(origin);
		vec2 sa = ResourceSystem::GetMainCamera()->WorldToScreenPoint(axis);
		vec2 sd = glm::normalize(sa - so);
		sd.y = -sd.y;

		vec2 mouseVector = mousePoint - m_hitScreenPoint;
		vec2 mouseDirec = normalize(mouseVector);
		mouseDirec.y = -mouseDirec.y;
		
		//计算与Direction夹角判断顺时针or逆时针
		vec3 res = cross(vec3(mouseDirec, 0.0), vec3(sd, 0.0));
		float sin = length(res);
		bool isClockwise = res.z < 0 ? true : false;
		float sign = isClockwise ? -1.0f : 1.0f;
		
		//屏幕上的len要还原到世界空间中的len
		float arc = sign * sin * length(mouseVector) * 0.3;

		m_axis->RotateTarget(arc * m_direction, true);
		InputSystem::GetMousePosition(m_hitScreenPoint.x, m_hitScreenPoint.y);
	}
	else
		m_isHit = false;
}
