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
	vector<vec3> m_lineVertics;
	bool m_isChanged = false;
	Transform *m_transform;

protected:
	LineDrawer(Transform *transform);

public:
	virtual void Draw();
	virtual void PublicSet();
	virtual int GetComponentType(void) { return (int)ComponentType::Drawer::LineDrawer; }

	void setOrigin(vec3 origin);
	void setDestination(vec3 dest);
	void SetRay(const Ray &ray, float len);
	void AddVertex(vec3 position);
	static LineDrawer* Create(Transform *transform = nullptr) { LineDrawer *instance = new LineDrawer(transform);instance->Register();return instance; }
};
