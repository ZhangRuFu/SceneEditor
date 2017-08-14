#include "RenderMode.h"
#include "Shader.h"
#include "ResourceSystem.h"
#include "Camera.h"

void DepthTestRenderMode::Set(void)
{
	glDepthFunc((GLenum)m_depthTestMode);
}

void DepthTestRenderMode::Reset(void)
{
	glDepthFunc(GL_LESS);
}

void CameraViewRenderMode::Set(void)
{
	m_shader->SetUniformValue("projection", ResourceSystem::GetMainCamera()->GenOrthoProjectionMatrix());
}

void CameraViewRenderMode::Reset(void)
{
	m_shader->SetUniformValue("projection", ResourceSystem::GetMainCamera()->GenProjectionMatrix());
}

void BlendRenderMode::Set(void)
{
	glEnable(GL_BLEND);
}

void BlendRenderMode::Reset(void)
{
	glDisable(GL_BLEND);
}
