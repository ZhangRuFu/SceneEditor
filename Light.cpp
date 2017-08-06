#include "Light.h"
#include "ResourceSystem.h"
#include "InputSystem.h"
#include "CommonType.h"

Light::Light(vec3 color) : m_color(color)
{
	ModelArg mArg(*this, BasicMesh::CUBE, color);
	AddComponent(mArg);
	m_isShow = true;
	ResourceSystem::Register(this);
}

vec3 Light::GetLightColor(void)
{
	return m_color;
}

void Light::CloseShow(void)
{
	m_isShow = false;
}

void Light::Move(void)
{
	if (InputSystem::isKeyDown(KEY_RIGHT))
		m_transform->Move(vec3(2, 0, 0));
	if (InputSystem::isKeyDown(KEY_UP))
		m_transform->Move(vec3(0, 2, 0));
}