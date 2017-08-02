#pragma once
/*
*	类名:RenderMode
*	描述：Drawer配置基类，对渲染模式进行更改
*	最后修改时间：2017年7月30日21:31:29
*
*/

#include <GL\glew.h>
#include "CommonType.h"

class RenderMode
{
public:
	virtual void Set(void) = 0;			//自定义设置
	virtual void Reset(void) = 0;		//恢复默认设置
	virtual RenderModeType GetRenderMode(void) = 0;
};

/*
*	类名:DepthTestRenderMode
*	描述：更改深度测试的渲染模式
*	最后修改时间：2017年7月31日19:03:09
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
