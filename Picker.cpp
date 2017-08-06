#include "Picker.h"
#include "TransformEditor.h"

Picker::Picker(void)
{
	m_axis = new TransformEditor();
	m_axis->Disable();
}

void Picker::Move()
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
			GameEntity *entity = hitInfo.getCollider()->GetEntity();
			//���������ỹ������
			if (entity->GetTag() == "TransformEditor")
				m_axis->Hit(*entity);
			else
				m_axis->SetTargetTransform(entity->GetTransform());
		}
		else
			m_axis->ReleaseTarget();
	}
}
