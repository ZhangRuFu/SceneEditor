#pragma once
#include "UI.h"
#include <string>


class Tiny2D;

/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：Activity
*	描　　述：游戏界面中当前活动界面，一个游戏在不能状态下有不同界面，可以建立多个Activity表示
*
*/
class Activity : public ViewGroup
{
public:
	Activity(string id);
	
	//Override
	void AcceptEvent(ivec2 mousePosition, MouseMotion mouseMotion);
	void AcceptEvent(int keyCode, KeyMotion keyMotion);
	void AcceptEvent(unsigned int codePoint);
	bool isInitLayout(void) { return m_isInitLayout; }
	void RequestFocus(View *view) { m_focusView = view; }

	virtual void OnCreate(void) { m_isInitLayout = true; }

private:
	bool m_isInitLayout;
	View *m_focusView;
	View *m_preFocusView;
};


/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：DragonActivity
*	描　　述：EdwardScissorhands的主界面
*
*/
class DragonActivity : public Activity
{
protected:
	TextView *m_texView;
	Button *m_btnAddClip;
	Button *m_btnExport;
	Button *m_btnImport;
	ClipBar *m_clipBar;
	ListView *m_listView;
	EditText *m_editPath;

public:
	DragonActivity(string id);
	virtual void OnCreate();
};

class AddClipListener : public MouseListener
{
private:
	ClipBar *m_clip;
	ListView *m_lstView;

public:
	AddClipListener(ClipBar *clip, ListView *lstView);

	virtual bool onMouse(View &view, const Event &e);
};

class ImportModelListener : public MouseListener
{
public:
	virtual bool onMouse(View &view, const Event &e);
};

class ExportXMLListener : public MouseListener
{
public:
	virtual bool onMouse(View &view, const Event &e);
};