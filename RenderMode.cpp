#include "RenderMode.h"

void DepthTestRenderMode::Set(void)
{
	glDepthFunc((GLenum)m_depthTestMode);
}

void DepthTestRenderMode::Reset(void)
{
	glDepthFunc(GL_LESS);
}
