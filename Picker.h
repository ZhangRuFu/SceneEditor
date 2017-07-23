#pragma once
#include "Entity.h"
#include "InputSystem.h"
#include "CommonType.h"
#include "ResourceSystem.h"
#include "Ray.h"
#include "CollideManager.h"
#include "LineRender.h"
#include <stdio.h>
class Picker : public GameSpirit
{
public:
	virtual void Move()
	{
		if (!InputSystem::isMouseKeyDown(MOUSE_KEY_LEFT))
			return;
		ivec2 clickPoint;
		InputSystem::GetMousePosition(clickPoint.x, clickPoint.y);
		Camera *camera = ResourceSystem::GetMainCamera();
		if (camera != nullptr)
		{
			Ray ray = camera->ScreenPointToRay(clickPoint);
			LineDrawer *drawer = (LineDrawer*)GetComponent((int)ComponentType::Drawer::LineDrawer);
			if (drawer == nullptr)
			{
				drawer = LineDrawer::Create();
				AddComponent(*drawer);
			}
			drawer->SetRay(ray, 100);
			RaycastHit hitInfo;
			if (CollideManager::RayCast(ray, &hitInfo))
				printf("»÷ÖÐ!\n");
			else
				printf("Î´»÷ÖÐ!\n");
		}
	}
};