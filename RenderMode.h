#pragma once
/*
*	����:RenderMode
*	������Drawer���û��࣬����Ⱦģʽ���и���
*	����޸�ʱ�䣺2017��7��30��21:31:29
*
*/

#include <GL\glew.h>
#include "CommonType.h"

class RenderMode
{
public:
	virtual void Set(void) = 0;			//�Զ�������
	virtual void Reset(void) = 0;		//�ָ�Ĭ������
	virtual RenderModeType GetRenderMode(void) = 0;
};

/*
*	����:DepthTestRenderMode
*	������������Ȳ��Ե���Ⱦģʽ
*	����޸�ʱ�䣺2017��7��31��19:03:09
*
*/
class DepthTestRenderMode : public RenderMode
{
public:
	enum class DepthTestMode
	{
		AlwaysPass = GL_ALWAYS,
		NeverPass = GL_NEVER,
		LessPass = GL_LESS,
		LessEqualPass = GL_LEQUAL,
		GreaterPass = GL_GREATER,
		GreaterEqualPass = GL_GEQUAL,
		EqualPass = GL_EQUAL,
		NotEqualPass = GL_NOTEQUAL
	};

private:
	DepthTestMode m_depthTestMode;

public:
	DepthTestRenderMode(int setting) : m_depthTestMode((DepthTestMode)setting) {}
	virtual void Set(void);
	virtual void Reset(void);
	virtual RenderModeType GetRenderMode(void) { return RenderModeType::DepthTest; }
};
