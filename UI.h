#pragma once
#include <list>
#include <string>
#include <GLM\glm.hpp>
#include <iostream>
#include "WindowSystem.h"

using std::list;
using std::string;
using glm::vec2;
using glm::vec3;
using glm::ivec2;

class Tiny2D;
class View;
class Activity;
class AnimationController;

/*
*	MouseListener ��������
*	KeyBoardListener ���̼�����
*/

class MouseListener
{
public:
	virtual bool onMouse(View &view, const Event &e) = 0;
};

class KeyBoardListener
{
public:
	virtual bool onKey(View &view, const Event &e) = 0;
};

/*
*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������View
*	�衡������UI�ؼ�����
*
*/

class View
{
	enum InvalidateType{ LayoutInvalidate = 0x01, MeasureInvalidate = 0x02};
public:
	enum Dimension{MATCH_PARENT = -2, WRAP_CONTENT = -1};

protected:
	string m_id;
	ivec2 m_relativePosition;					//����ڸ�View�����λ��
	ivec2 m_fatherPosition;						//����Viewλ��
	int m_layoutWidth, m_layoutHeight;			//���ֲ���
	int m_width, m_height;						//����
	unsigned char m_invalidateState;			//��Ч״̬
	View *m_parentView;							//������ͼ
	Activity *m_activity;						//����Activity

	//������
	MouseListener *m_mouseListener;
	KeyBoardListener *m_keyListener;

public:
	View(Activity *activity, const string &id, ivec2 &position, int layoutWidth, int layoutHeight);
	
	//�麯��
	virtual bool DispatchEvent(Event &ievent);						//�¼��ַ�
	virtual void OnMeasure(int fatherWidth, int fatherHeight);		//�ߴ�ⶨ
	virtual void OnPosit(int fatherX, int fatherY);					//λ�òⶨ
	virtual void OnDraw(Tiny2D *paint) = 0;							//����

	//λ��
	void SetFatherPosition(ivec2 fatherPosition) { m_fatherPosition = fatherPosition; }
	void SetPosition(ivec2 position) { m_relativePosition = position; }
	void SetPositionX(int x) { m_relativePosition.x = x; }
	void SetPositionY(int y) { m_relativePosition.y = y; }
	void AddPosition(int x, int y) { m_relativePosition.x += x, m_relativePosition.y += y; }
	void SetSize(int width, int height) { m_width = width; m_height = height; }
	void SetDimension(int width, int height) { m_width = width, m_height = height; }

	ivec2 GetAbsolutePosition(void) const { return m_relativePosition + m_fatherPosition; }
	ivec2 GetPosition(void) const { return m_relativePosition; }
	int GetPositionY(void) const { return m_relativePosition.y; }
	int GetPositionX(void) const { return m_relativePosition.x; }
	ivec2 GetDimension(void) const { return ivec2(m_width, m_height); }
	int GetWidth(void) const { return m_width; }
	int GetHeight(void) const { return m_height; }
	
	string GetViewID(void) { return m_id; }
	void SetParentView(View *parent);
	bool HitView(Event & ievent);
	Activity* GetActivity(void) { return m_activity; }
	void RequestFocus(void);

	//������Ч
	void SetMeasureInvalidate(void) { m_invalidateState |= InvalidateType::MeasureInvalidate; }
	void SetLayoutInvalidate(void) { m_invalidateState |= InvalidateType::LayoutInvalidate; }
	bool isMeasureInvalidate(void) { return ((m_invalidateState & InvalidateType::MeasureInvalidate) != 0); }
	bool isLayoutInvalidate(void) { return ((m_invalidateState & InvalidateType::LayoutInvalidate) != 0); }
	void Invalidate(View *invalidateView);

	//�¼�����
	void SetMouseListener(MouseListener *mouseListener) { m_mouseListener = mouseListener; }
	void SetKeyListener(KeyBoardListener *keyListener) { m_keyListener = keyListener; }
};

/*
*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������ViewGroup
*	�衡������View�������ɰ������View��ListView��ListItem�̳����ˡ�Ҳ���ǿ����á���չ�Ŀؼ�
*
*/

class ViewGroup : public View
{
protected:
	list<View*> m_viewList;

public:
	//ViewGroup�Ŀ�Ⱥ͸߶Ȳ���ΪWRAP_CONTENT
	ViewGroup(Activity *activity, const string id, ivec2 position, int width, int height) : View(activity, id, position, width, height) {}
	virtual bool DispatchEvent(Event &ievent);						//�¼��ַ�
	virtual void OnMeasure(int fatherWidth, int fatherHeight);		//�ߴ�ⶨ
	virtual void OnPosit(int fatherX, int fatherY);					//λ�òⶨ
	virtual void OnDraw(Tiny2D *paint);								//����
	void AddView(View *view);
	View* FindViewByID(string id);
};




//============================================����UI===========================================

/*
*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������TextView
*	�衡������������ʾ�ؼ�
*
*/

class TextView : public View
{
public:
	enum TextAligin{Top = 1, Bottom = 2, Left = 4, Right = 8, Center = 15};

protected:
	string m_text;
	int m_fontSize;
	vec3 m_fontColor;
	ivec2 m_fontPosition;
	int m_texWidth, m_texHeight;
	TextAligin m_texAlign;

public:
	TextView(Activity *activity, const string id, ivec2 position, string text, int width = Dimension::WRAP_CONTENT, int height = Dimension::WRAP_CONTENT, TextAligin texAlign = TextAligin::Center, vec3 color = vec3(1.0, 1.0, 1.0), int fontSize = 16);

	//Override
	virtual void OnMeasure(int fatherWidth, int fatherHeight);
	virtual void OnPosit(int fatherX, int fatherY);
	virtual void OnDraw(Tiny2D *paint);

	string GetText(void) const { return m_text; }
	void SetText(string text) { m_text = text; SetMeasureInvalidate(); Invalidate(this); }
	vec3 GetFontColor(void) const { return m_fontColor; }
	void SetFontColor(vec3 color) { m_fontColor = color; }                      
	int GetFontSize(void) const { return m_fontSize; }
	void SetFontSize(int fontSize) { m_fontSize = fontSize; SetMeasureInvalidate(); Invalidate(this); }
	TextAligin GetTextAlign(void) { return m_texAlign; }
	void SetTextAlign(TextAligin texAlign) { m_texAlign = texAlign; SetLayoutInvalidate(); Invalidate(this); }
};

/*
*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������EditText
*	�衡���������ֱ༭�ؼ�
*
*/

class EditText : public TextView
{
public:
	EditText(Activity *activity, const string id, ivec2 position, string text = "", int width = Dimension::WRAP_CONTENT, int height = Dimension::WRAP_CONTENT, TextAligin texAlign = TextAligin::Center, vec3 color = vec3(1.0, 1.0, 1.0), int fontSize = 16);

	//Override
	virtual void OnDraw(Tiny2D *paint);
	virtual bool DispatchEvent(Event &ievent);
	virtual void OnMeasure(int fatherWidth, int fatherHeight);
	virtual void OnPosit(int fatherX, int fatherY);

private:
	static const int TBPadding = 5;
	static const int LRPadding = 10;
};

/*
*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������Button
*	�衡��������ť
*
*/

enum ButtonState { Normal, Focus, Clicked };

class Button : public TextView
{
private:
	ButtonState m_state;
	const int LRPadding = 15;
	const int TBPadding = 10;

public:
	Button(Activity *activity, const string &id, ivec2 position, string text, int width = View::Dimension::WRAP_CONTENT, int height = View::Dimension::WRAP_CONTENT);
	
	//Override
	virtual void OnDraw(Tiny2D *paint);
	virtual void OnPosit(int fatherX, int fatherY);
	virtual void OnMeasure(int fatherWidth, int fatherHeight);
	virtual bool DispatchEvent(Event &ievent);

	ButtonState GetButtonState(void) const { return m_state; }
};

/*
*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������ClipBar
*	�衡������Ƭ��ѡ��
*
*/

class ClipBarMeasure
{
public:
	static const int m_lenToTop = 20;
	static const int m_leftOffset = 20;
	static const int m_rightOffset = 20;
	static const int m_interval = 10;
	static const int m_axisToStart = 20;		//
	static const int m_axisLen = 210;			//�᳤��
	static const int m_slideLen = 10;			//����߳�
	static const int m_slideToBottom = 10;		//�������ײ�����
};

class ClipBar : public View
{
public:
	ClipBar(Activity *activity, string id, ivec2 position, AnimationController *controller = nullptr, int width = 250, int height = 170);		//���캯��
	int GetStartValue(void) { return m_start; }
	int GetEndValue(void) { return m_end; }
	void SetStartValue(int value);
	void AddStartValue(int delta) { SetStartValue(delta + m_start); }
	void AddEndValue(int delta) { SetEndValue(m_end + delta); }
	void SetEndValue(int value);
	int GetLength(void) { return m_length; }
	void SetLength(int len);
	string GetClipName(void) { return m_editClipName->GetText(); }
	int GetSlideMinX(void) { return m_minPositionX; }
	int GetSlideMaxX(void) { return m_maxPositionX; }
	void SetAnimationController(AnimationController *controller);
	AnimationController* GetAnimationController(void) { return m_animationController; }

	//Override
	virtual void OnDraw(Tiny2D *paint);
	virtual void OnMeasure(int fatherWidth, int fatherHeight);
	virtual void OnPosit(int fatherWidth, int fatherHeight);
	virtual bool DispatchEvent(Event &ievent);

private:
	bool isStart(void) { return m_isStart; }

private:
	int m_length;
	int m_start;
	int m_end;
	ivec2 m_axisPosition;

	EditText *m_editClipName;
	TextView *m_startText;
	TextView *m_endText;
	TextView *m_lenText;
	Button *m_btnStart;
	Button *m_btnEnd;
	Button *m_btnPlay;
	Button *m_btnAdd;
	Button *m_btnMinus;

	bool m_isStart;

	int m_minPositionX, m_maxPositionX;
	AnimationController *m_animationController;

	class ClipButtonListener : public MouseListener
	{
	private:
		ClipBar *m_clipBar;

	public:
		ClipButtonListener(ClipBar *clipBar) { m_clipBar = clipBar; }

		virtual bool onMouse(View &view, const Event &e)
		{
			string id = view.GetViewID();
			if (id == "ClipBar.Play")
				return PlayButton(view, e);
			else if (id == "ClipBar.AddButton")
				return AddMinusButton(true, e);
			else if (id == "ClipBar.MinusButton")
				return AddMinusButton(false, e);
			else
				return StartEndButton(view, e);
		}
	
		bool AddMinusButton(bool isAdd, const Event &e);

		bool PlayButton(View &view, const Event &e);

		bool StartEndButton(View &view, const Event &e);
	};
};



/*
*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������ListItem
*	�衡������ListView��Item�Ļ���
*
*/
class ListItem : public View
{
public:
	ListItem(Activity *activity, string id = "", ivec2 position = vec2(0, 0), int width = View::Dimension::MATCH_PARENT, int height = View::Dimension::WRAP_CONTENT) : View(activity, id, position, width, height) {};
};

/*
*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������ClipItem
*	�衡���������Ƭ�����ݵ�ListItem
*
*/
class ClipItem : public ListItem
{
private:
	string m_clipName;
	float m_start;
	float m_end;

	static const int Interval = 10;
	static const int LeftPadding = 10;
	static const int RightPadding = 10;
	static const int TopPadding = 10;
	static const int BottomPadding = 10;

	TextView *m_texClip;
	TextView *m_texStart;
	TextView *m_texEnd;

public:
	ClipItem(Activity *activity, string clipName, float start, float end);

	//Override
	virtual void OnDraw(Tiny2D *paint);
	virtual void OnMeasure(int fatherWidth, int fatherHeight);
	virtual void OnPosit(int fatherWidth, int fatherHeight);
	float GetStart(void) { return m_start; }
	float GetEnd(void) { return m_end; }
	string GetClipName(void) { return m_clipName; }
};

/*
*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������ListView
*	�衡�������б���ͼ
*
*/
class ListView : public View
{
	static const int LRPadding = 5;
	static const int TBPadding = 5;
private:
	list<ListItem*> m_items;

public:
	ListView(Activity *activity, string id, ivec2 position, int width = 200, int height = 400);
	void AddItem(ListItem* item);
	list<ListItem*> GetItems(void) const { return m_items; }

	//Override
	virtual void OnDraw(Tiny2D *paint);
	virtual void OnMeasure(int fatherWidth, int fatherHeight);
	virtual void OnPosit(int fatherWidth, int fatherHeight);
	virtual bool DispatchEvent(Event &ievent);
};