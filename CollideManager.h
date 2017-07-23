#pragma once
#include <list>
#include <map>
#include "Ray.h"

/*
*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������CollideManager
*	�衡������	������Ϸ�е�������ײ�壬
				����Ϸģʽ�У�ÿ��Move��Ҫ�������֮�����޷�����ײ
				�ڳ����༭��ģʽ�У��������嶼�Ǿ�̬������������
				�������н�����ײ����ײ����⣬�ڽ���ʰȡ��FPS��Ϸ�е����߼���ͻ������ײ���ҲҪͨ������ӿ�������
*
*/

using std::list;
using std::map;
class Collider;
class Model;
class GameEntity;
class ResourceSystem;

class CollideManager
{
private:
	static CollideManager *m_instance;

	ResourceSystem *m_resourceSystem;
	list<Collider*> m_colliders;			//��Ϸ��������ײ��
	map<int, Collider*> m_modelBoundingMap; //ģ��ID-��Χ��ӳ�����ֹ�ظ�����ģ�Ͱ�Χ�У�

public:
	CollideManager(ResourceSystem *rSystem);

	void CalcModelBoundingBox(Model* model);	//MeshManager������ģ��ʱ���ô˽ӿ�
	void Register(GameEntity *entity);			//ע����ײ��
	void Move(void) {}
	bool _RayCast(const Ray &ray, RaycastHit *hitInfo, float len);

	static CollideManager* GetInstance(ResourceSystem *rSystem);
	static bool RayCast(const Ray &ray, RaycastHit *hitInfo);
	static bool RayCast(const Ray &ray, RaycastHit *hitInfo, float len);
};