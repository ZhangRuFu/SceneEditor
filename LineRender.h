#pragma once
#include "Drawer.h"
#include "CommonType.h"
#include "Ray.h"
#include <GLM\glm.hpp>
#include <string>

using std::string;
using glm::vec3;

class LineDrawer : public Drawer
{
private:
	vec3 m_origin;
	vec3 m_dest;

protected:
	LineDrawer();

public:
	virtual void Draw();
	virtual void PublicSet();
	virtual RenderLevel GetRenderLevel() { return RenderLevel::Entity; };
	virtual int GetComponentType(void) { return (int)ComponentType::Drawer::LineDrawer; }

	void setOrigin(vec3 origin);
	void setDestination(vec3 dest);
	void SetRay(const Ray &ray, float len);
	static LineDrawer* Create(void) { LineDrawer *instance = new LineDrawer();instance->Register();return instance; }
};
