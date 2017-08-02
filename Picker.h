#pragma once
#include "Entity.h"
#include "InputSystem.h"
#include "CommonType.h"
#include "ResourceSystem.h"
#include "Ray.h"
#include "CollideManager.h"
#include "LineRender.h"
#include "TransformEditor.h"
#include <stdio.h>
class Picker : public GameSpirit
{
private:
	TranslateAxis *m_axis = nullptr;

public:
	Picker(TranslateAxis *axis) : m_axis(axis) {}

	virtual void Move()
	{
		if (!InputSystem::isOnMouseKeyDown(MOUSE_KEY_LEFT))
			return;
		ivec2 clickPoint;
		InputSystem::GetMousePosition(clickPoint.x, clickPoint.y);
		Camera *camera = ResourceSystem::GetMainCamera();
		if (camera != nullptr)
		{
			Ray ray = camera->ScreenPointToRay(clickPoint);
			
			//���ӻ�����
			/*LineDrawer *drawer = (LineDrawer*)GetComponent((int)ComponentType::Drawer::LineDrawer);
			if (drawer == nullptr)
			{
				drawer = LineDrawer::Create();
				AddComponent(*drawer);
			}
			drawer->SetRay(ray, 100);*/
			
			//���߼��
			RaycastHit hitInfo;
			if (CollideManager::RayCast(ray, &hitInfo))
			{
				std::cout << "����" << std::endl;
				GameEntity *entity = hitInfo.getCollider()->GetEntity();
				TranslateSubAxis* axis = dynamic_cast<TranslateSubAxis*>(entity);
				//�Ƿ����������
				if (axis != nullptr)
					axis->Hit();
				else
					m_axis->setTargetTransform(entity->GetTransform());
			}
			else
				m_axis->ReleaseTarget();
		}
	}
};