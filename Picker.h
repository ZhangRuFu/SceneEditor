#pragma once
#include "Entity.h"
#include "InputSystem.h"
#include "CommonType.h"
#include "ResourceSystem.h"
#include "Ray.h"
#include "CollideManager.h"
#include "LineRender.h"

class TransformEditor;

class Picker : public GameSpirit
{
private:
	TransformEditor *m_axis = nullptr;

public:
	Picker(void);

	virtual void Move();
};