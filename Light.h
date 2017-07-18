#pragma once
#include "Entity.h"
#include "SimpleModelDrawer.h"
#include "MeshManager.h"

class Light : public GameSpirit
{
private:
	bool m_isShow;
	vec3 m_color;

public:
	Light(vec3 color = vec3(1.0, 1.0, 1.0));
	
	vec3 GetLightColor(void);
	void CloseShow(void);

	void Move();
};