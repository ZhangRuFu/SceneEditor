#pragma once
#include <list>
#include <map>
#include "Ray.h"

/*
*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：CollideManager
*	描　　述：	管理游戏中的所有碰撞体，
				在游戏模式中，每次Move都要检测物体之间有无发生碰撞
				在场景编辑器模式中，由于物体都是静态，所以无需检测
				除了自行进行碰撞体碰撞检测外，在进行拾取、FPS游戏中的射线检测等突发性碰撞检测也要通过该类接口申请检测
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
	list<Collider*> m_colliders;			//游戏中所有碰撞体
	map<int, Collider*> m_modelBoundingMap; //模型ID-包围盒映射表（防止重复计算模型包围盒）

public:
	CollideManager(ResourceSystem *rSystem);

	void CalcModelBoundingBox(Model* model);	//MeshManager加载新模型时调用此接口
	void Register(GameEntity *entity);			//注册碰撞体
	void Move(void) {}
	bool _RayCast(const Ray &ray, RaycastHit *hitInfo, float len);

	static CollideManager* GetInstance(ResourceSystem *rSystem);
	static bool RayCast(const Ray &ray, RaycastHit *hitInfo);
	static bool RayCast(const Ray &ray, RaycastHit *hitInfo, float len);
};