//#pragma once
//#include <list>
//#include "CommonType.h"
//#include "Shader.h"
//#include "Tiny2D.h"
//#include <GLM\glm.hpp>
//
//class View;
//class Button;
//class TextView;
//class ClipBar;
//class Tiny2D;
//class ListView;
//class ClipItem;
//
//using std::list;
//
///*
//*
//*	引擎版本：Dragon Engine v0.1;
//*	类　　名：UIDrawer
//*	描　　述：UI绘制器基类，使用Tiny2D提供的接口进行绘制
//*
//*/
//
//class UIDrawer : public Tiny2D
//{
//public:
//	UIDrawer(void);
//
//	virtual void Draw() = 0;
//	virtual RenderLevel GetRenderLevel(void) { return RenderLevel::UI; }
//
//	void Register() { Drawer::Register(); }
//};
//
///*
//*
//*	引擎版本：Dragon Engine v0.1;
//*	类　　名：TextViewDrawer
//*	描　　述：TextView绘制器
//*
//*/
//class TextViewDrawer : public UIDrawer
//{
//public:
//	const TextView *m_texView;
//	TextViewDrawer(const TextView *texView);
//
//public:
//	virtual void Draw(void);
//};
//
///*
//*
//*	引擎版本：Dragon Engine v0.1;
//*	类　　名：ButtonDrawer
//*	描　　述：Button绘制器
//*
//*/
//class ButtonDrawer : public UIDrawer
//{
//protected:
//	const Button *m_button;
//	TextViewDrawer *m_textDrawer;
//
//	
//
//public:
//	ButtonDrawer(const Button *button);
//	virtual void Draw(void);
//};
//
//
///*
//*
//*	引擎版本：Dragon Engine v0.1;
//*	类　　名：ClipBarDrawer
//*	描　　述：ClipBar绘制器
//*
//*/
//
//class ClipBarDrawer : public UIDrawer
//{
//protected:
//	const ClipBar *m_clipBar;
//	TextViewDrawer *m_startText;
//	TextViewDrawer *m_endText;
//	TextViewDrawer *m_lenText;
//	ButtonDrawer *m_startButton;
//	ButtonDrawer *m_endButton;
//	
//	ivec2 m_axisPosition;
//
//public:
//	virtual void Draw(void);
//	ClipBarDrawer(const ClipBar *clipBar);
//};
//
//
///*
//*
//*	引擎版本：Dragon Engine v0.1;
//*	类　　名：ClipItemDrawer
//*	描　　述：ClipItem绘制器
//*
//*/
//
//class ClipItemDrawer : public UIDrawer
//{
//private:
//	ClipItem *m_clipItem;
//	TextViewDrawer *m_texClip;
//	TextViewDrawer *m_texStart;
//	TextViewDrawer *m_texEnd;
//
//public:
//	ClipItemDrawer(ClipItem *clipItem);
//
//	virtual void Draw(void);
//	
//
//};
//
///*
//*
//*	引擎版本：Dragon Engine v0.1;
//*	类　　名：ListViewDrawer
//*	描　　述：ListView绘制器
//*
//*/
//
//class ListViewDrawer : public UIDrawer
//{
//private:
//	const ListView *m_listView;
//	list<UIDrawer*> m_itemDrawers;
//
//public:
//	ListViewDrawer(const ListView *listView);
//	virtual void Draw();
//};