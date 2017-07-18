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
//*	����汾��Dragon Engine v0.1;
//*	�ࡡ������UIDrawer
//*	�衡������UI���������࣬ʹ��Tiny2D�ṩ�Ľӿڽ��л���
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
//*	����汾��Dragon Engine v0.1;
//*	�ࡡ������TextViewDrawer
//*	�衡������TextView������
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
//*	����汾��Dragon Engine v0.1;
//*	�ࡡ������ButtonDrawer
//*	�衡������Button������
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
//*	����汾��Dragon Engine v0.1;
//*	�ࡡ������ClipBarDrawer
//*	�衡������ClipBar������
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
//*	����汾��Dragon Engine v0.1;
//*	�ࡡ������ClipItemDrawer
//*	�衡������ClipItem������
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
//*	����汾��Dragon Engine v0.1;
//*	�ࡡ������ListViewDrawer
//*	�衡������ListView������
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