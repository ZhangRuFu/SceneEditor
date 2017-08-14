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
	friend class RenderSystem;
public:
	enum BasicLine{Ring, Count};
	enum LineType{LINES, LOOP_LINE, STRIIP_LINE};
	
private:
	vector<vec3> m_lineVertics;
	bool m_isChanged = false;
	Transform *m_transform;
	unsigned int m_type = GL_LINES;
	vec3 m_color;

	static LineDrawer *m_basicLine;

public:
	LineDrawer(void);
	LineDrawer(BasicLine line);
	virtual void SetEntity(GameEntity &entity);
	virtual void Draw();
	virtual void PublicSet();
	virtual int GetComponentType(void) { return (int)ComponentType::Drawer::LineDrawer; }

	void setOrigin(vec3 origin);
	void setDestination(vec3 dest);
	void SetRay(const Ray &ray, float len);
	void AddVertex(vec3 position);
	void ChangeLineType(LineType type);
	void SetColor(vec3 color) { m_color = color; }
	vec3 GetColor(void) { return m_color; }

private:
	static void InitBasicLine(void);
};