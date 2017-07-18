#include "UI.h"
#include "Tiny2D.h"
#include "Activity.h"
#include "MathUtility.h"
#include "AnimationController.h"
#include <GLM\gtx\transform.hpp>

View::View(Activity *activity, const string &id, ivec2 &position, int layoutWidth, int layoutHeight) : m_id(id), m_relativePosition(position)
{
	m_layoutWidth = layoutWidth;
	m_layoutHeight = layoutHeight;
	m_invalidateState = 0;
	SetMeasureInvalidate();
	SetLayoutInvalidate();
	m_width = m_height = 0;
	m_activity = activity;
	m_mouseListener = nullptr;
	m_keyListener = nullptr;
	m_parentView = nullptr;
}

bool View::DispatchEvent(Event & ievent)
{
	//View的事件分发
	//是否在此View区域
	if (!HitView(ievent))
		return false;
	if (ievent.m_mouseMotion == MouseMotion::LeftButtonDown && m_activity != nullptr)
		RequestFocus();
	return true;
}

void View::OnMeasure(int fatherWidth, int fatherHeight)
{
	//宽度
	if (m_layoutWidth == Dimension::MATCH_PARENT)
		m_width = fatherWidth;
	else if (m_layoutWidth >= 0 )
		m_width = m_layoutWidth;
	//高度
	if (m_layoutHeight == Dimension::MATCH_PARENT)
		m_height = fatherHeight;
	else if (m_layoutHeight >= 0)
		m_height = m_layoutHeight;
	m_invalidateState &= (~InvalidateType::MeasureInvalidate);
}

void View::OnPosit(int fatherX, int fatherY)
{
	m_fatherPosition.x = fatherX;
	m_fatherPosition.y = fatherY;
	m_invalidateState &= (~InvalidateType::LayoutInvalidate);
}

void View::SetParentView(View * parent)
{
	m_parentView = parent;
	SetFatherPosition(m_parentView->GetAbsolutePosition());
}

//返回是否击中此View
bool View::HitView(Event & ievent)
{
	int startX = m_fatherPosition.x + m_relativePosition.x;
	int endX = startX + m_width;
	int startY = m_fatherPosition.y + m_relativePosition.y;
	int endY = startY + m_height;
	if (ievent.m_mousePosition.x < startX || ievent.m_mousePosition.x > endX ||
		ievent.m_mousePosition.y < startY || ievent.m_mousePosition.y > endY)
		//不在View区域
		return false;
	//击中此View
	return true;
}

void View::RequestFocus(void)
{
	m_activity->RequestFocus(this);
}

void View::Invalidate(View *invalidateView)
{
	m_invalidateState = invalidateView->m_invalidateState;
	//如果Activity并未进行第一次布局，不理会这次请求
	if (!m_activity->isInitLayout())
		return;
	
	//如果父View为nullptr
	//两种情况，第一种为顶层窗口，Activity
	//第二种情况，出现错误（概率较低）
	//统一处理为，把自己的m_width和m_height传入
	ivec2 fatherDimension;
	ivec2 fatherPosition;
	if (m_parentView == nullptr)
	{
		fatherDimension.x = m_width;
		fatherDimension.y = m_height;
		fatherPosition = ivec2(0, 0);

		//顶层窗口向下传递
		//出错，只能向下传递
		if(isMeasureInvalidate())
			OnMeasure(fatherDimension.x, fatherDimension.y);
		if(isLayoutInvalidate())
			OnPosit(fatherPosition.x, fatherPosition.y);
		return;
	}
	else
	{
		fatherDimension = m_parentView->GetDimension();
		fatherPosition = m_parentView->GetAbsolutePosition();
	}

	//如果是绝对布局，自行解决
	if (m_layoutWidth >= 0 && m_layoutHeight >= 0)
	{
		if (isMeasureInvalidate())
			OnMeasure(fatherDimension.x, fatherDimension.y);
		if (isLayoutInvalidate())
			OnPosit(fatherPosition.x, fatherPosition.y);
		return;
	}

	//其他情况，只能向上汇报
	m_parentView->Invalidate(this);
}




//====================================================ViewGroup===========================================
bool ViewGroup::DispatchEvent(Event & ievent)
{
	//ViewGroup的事件分发
	if (!View::DispatchEvent(ievent))
		return false;

	for (list<View*>::iterator i = m_viewList.begin(); i != m_viewList.end(); i++)
		if ((*i)->DispatchEvent(ievent))
			return true;
	return true;
}

void ViewGroup::OnMeasure(int fatherWidth, int fatherHeight)
{
	View::OnMeasure(fatherWidth, fatherHeight);
	for (list<View*>::iterator i = m_viewList.begin(); i != m_viewList.end(); i++)
		(*i)->OnMeasure(m_width, m_height);
}

void ViewGroup::OnPosit(int fatherX, int fatherY)
{
	View::OnPosit(fatherX, fatherY);
	ivec2 aPosition = GetAbsolutePosition();
	for (list<View*>::iterator i = m_viewList.begin(); i != m_viewList.end(); i++)
		(*i)->OnPosit(aPosition.x, aPosition.y);
}

void ViewGroup::OnDraw(Tiny2D * paint)
{
	for (list<View*>::iterator i = m_viewList.begin(); i != m_viewList.end(); i++)
		(*i)->OnDraw(paint);
}

void ViewGroup::AddView(View *view)
{
	m_viewList.push_back(view);
	view->SetParentView(this);
	Invalidate(this);
}

View* ViewGroup::FindViewByID(string id)
{
	for (list<View*>::iterator i = m_viewList.begin(); i != m_viewList.end(); i++)
		if ((*i)->GetViewID() == id)
			return (*i);
	return nullptr;
}




//====================================================TextView===========================================
TextView::TextView(Activity *activity, const string id, ivec2 position, string text, int width, int height, TextAligin texAlign, vec3 color, int fontSize) : View(activity, id, position, width, height), m_text(text)
{
	m_texAlign = texAlign;
	m_fontColor = color;
	m_fontSize = fontSize;
	m_texWidth = m_texHeight = 0;
}

void TextView::OnMeasure(int fatherWidth, int fatherHeight)
{
	FontRender::GetDimension(m_text, m_texWidth, m_texHeight);
	View::OnMeasure(fatherWidth, fatherHeight);
	if (m_layoutWidth == View::Dimension::WRAP_CONTENT)
		m_width = m_texWidth;
	if (m_layoutHeight == View::Dimension::WRAP_CONTENT)
		m_height = m_texHeight;
	
}

void TextView::OnPosit(int fatherX, int fatherY)
{
	View::OnPosit(fatherX, fatherY);
	m_fontPosition.x = m_fontPosition.y = 0;
}

void TextView::OnDraw(Tiny2D * paint)
{
	ivec2 texPosition = GetAbsolutePosition();
	texPosition.x += m_fontPosition.x;
	texPosition.y += m_fontPosition.y;
	paint->DrawText(m_text, texPosition, m_fontSize, m_fontColor);
}




//====================================================Button===========================================
Button::Button(Activity *activity, const string &id, ivec2 position, string text, int width, int height) : TextView(activity, id, position, text, width, height)
{
	m_state = ButtonState::Normal;
}

void Button::OnDraw(Tiny2D * paint)
{
	static glm::vec3 buttonColor = vec3(0.0f, 0.0f, 0.9f);
	static glm::vec3 offset = vec3(0.1f, 0.1f, 0.1f);
	vec3 color = buttonColor;
	if (m_state == ButtonState::Focus)
		color += offset;
	else if (m_state == ButtonState::Clicked)
		color -= offset;

	ivec2 position = GetAbsolutePosition();
	paint->DrawRect(position, m_width, m_height, color);
	TextView::OnDraw(paint);
}

void Button::OnPosit(int fatherX, int fatherY)
{
	TextView::OnPosit(fatherX, fatherY);
	m_fontPosition.x = (m_width - m_texWidth) / 2.0;
	m_fontPosition.y = (m_height - m_texHeight) / 2.0;
}

void Button::OnMeasure(int fatherWidth, int fatherHeight)
{
	TextView::OnMeasure(fatherWidth, fatherHeight);
	if (m_layoutWidth == View::Dimension::WRAP_CONTENT)
		m_width += LRPadding * 2;
	if (m_layoutHeight == View::Dimension::WRAP_CONTENT)
		m_height += TBPadding * 2;
	//宽高小于父View宽高怎么办？？？？？？
}

bool Button::DispatchEvent(Event & ievent)
{
	if (!View::DispatchEvent(ievent))
	{
		m_state = ButtonState::Normal;
		return false;
	}
	if (ievent.m_mouseMotion == MouseMotion::MouseMove)
		m_state = ButtonState::Focus;
	else if (ievent.isMouseKeyDown())
		m_state = ButtonState::Clicked;
	else if(ievent.isMouseKeyUp())
		m_state = ButtonState::Normal;

	if (m_mouseListener != nullptr)
		m_mouseListener->onMouse(*this, ievent);

	return true;
}





//====================================================ClipBar===========================================
ClipBar::ClipBar(Activity *activity, string id, ivec2 position, AnimationController *controller, int width, int height) : View(activity, id, position, width, height)
{
	m_isStart = true;
	m_animationController = controller;
	m_length = 0;
	m_start = 0;
	m_end = m_length;
	m_minPositionX = m_maxPositionX = 0;
	char tex[20];
	sprintf(tex, "Length: %d", 0);
	m_lenText = new TextView(m_activity, "", ivec2(0, 0), tex);
	m_lenText->SetParentView(this);
	m_editClipName = new EditText(m_activity, "", ivec2(0, 0), "ClipName_01");
	sprintf(tex, "Start: %d", m_start);
	m_startText = new TextView(m_activity, "", ivec2(0, 0), tex);
	m_startText->SetParentView(this);
	sprintf(tex, "End: %d", m_end);
	m_endText = new TextView(m_activity, id, ivec2(0, 0), tex);
	m_endText->SetParentView(this);

	m_btnStart = new Button(m_activity, "ClipBar.StartButton", ivec2(0, 0), "", ClipBarMeasure::m_slideLen, ClipBarMeasure::m_slideLen);
	m_btnStart->SetParentView(this);
	
	m_btnEnd = new Button(m_activity, "ClipBar.EndButton", ivec2(0, 0), "", ClipBarMeasure::m_slideLen, ClipBarMeasure::m_slideLen);
	m_btnEnd->SetParentView(this);

	m_btnPlay = new Button(m_activity, "ClipBar.Play", ivec2(0, 0), "Play", View::Dimension::WRAP_CONTENT, View::Dimension::WRAP_CONTENT);
	m_btnPlay->SetParentView(this);

	m_btnAdd = new Button(m_activity, "ClipBar.AddButton", ivec2(0, 0), ">", 15, 15);
	m_btnAdd->SetParentView(this);

	m_btnMinus = new Button(m_activity, "ClipBar.MinusButton", ivec2(0, 0), "<", 15, 15);
	m_btnMinus->SetParentView(this);


	//==================================================内存泄漏===================================
	ClipButtonListener *listener = new ClipButtonListener(this);
	m_btnStart->SetMouseListener(listener);
	m_btnEnd->SetMouseListener(listener);
	m_btnPlay->SetMouseListener(listener);
	m_btnAdd->SetMouseListener(listener);
	m_btnMinus->SetMouseListener(listener);
}

void ClipBar::SetStartValue(int value)
{
	char str[20];
	if (value == m_start || m_length == 0)
		return;
	value = value < 0 ? 0 : (value >= m_length ? m_length - 1 : value);
	sprintf(str, "Start:%d", value);
	m_start = value;
	m_startText->SetText(str);
	if (m_animationController != nullptr)
		m_animationController->SetKey(value);
}

void ClipBar::SetEndValue(int value)
{
	char str[20];
	if (value == m_end || m_length == 0)
		return;
	value = value >= m_length ? m_length - 1 : (value < 0 ? 0 : value);
	sprintf(str, "End:%d", value);
	m_end = value;
	m_endText->SetText(str);
	if (m_animationController != nullptr)
		m_animationController->SetKey(value);
}

void ClipBar::SetLength(int len)
{
	char tex[20];
	m_length = len;
	sprintf(tex, "Length: %d", m_length);
	SetStartValue(0);
	SetEndValue(len-1);
	m_lenText->SetText(tex);
}

void ClipBar::SetAnimationController(AnimationController * controller)
{
	m_animationController = controller;
	SetLength(controller->GetLength());
}

void ClipBar::OnDraw(Tiny2D * paint)
{
	ivec2 rt = GetAbsolutePosition();
	paint->DrawRoundRect(rt, m_width, m_height, 15, vec3(0.7f, 0.1f, 0.2f));
	rt.x += m_axisPosition.x;
	rt.y += m_axisPosition.y;
	paint->DrawLine(vec2(rt.x, rt.y), vec2(rt.x + ClipBarMeasure::m_axisLen, rt.y));
	paint->DrawLine(vec2(rt.x, rt.y - 5), vec2(rt.x, rt.y + 5));
	paint->DrawLine(vec2(rt.x + ClipBarMeasure::m_axisLen, rt.y - 5), vec2(rt.x + ClipBarMeasure::m_axisLen, rt.y + 5));
	
	m_startText->OnDraw(paint);
	m_endText->OnDraw(paint);
	m_lenText->OnDraw(paint);
	m_btnStart->OnDraw(paint);
	m_btnEnd->OnDraw(paint);
	m_btnPlay->OnDraw(paint);
	m_editClipName->OnDraw(paint);
	m_btnAdd->OnDraw(paint);
	m_btnMinus->OnDraw(paint);
}

void ClipBar::OnMeasure(int fatherWidth, int fatherHeight)
{
	View::OnMeasure(fatherWidth, fatherHeight);
	m_lenText->OnMeasure(m_width, m_height);
	m_startText->OnMeasure(m_width, m_height);
	m_endText->OnMeasure(m_width, m_height);
	m_btnStart->OnMeasure(m_width, m_height);
	m_btnEnd->OnMeasure(m_width, m_height);
	m_editClipName->OnMeasure(m_width, m_height);
	m_btnPlay->OnMeasure(m_width, m_height);
	m_btnAdd->OnMeasure(m_width, m_height);
	m_btnMinus->OnMeasure(m_width, m_height);
}

void ClipBar::OnPosit(int fatherWidth, int fatherHeight)
{
	View::OnPosit(fatherWidth, fatherHeight);
	ivec2 aP = GetAbsolutePosition();
	m_lenText->OnPosit(aP.x, aP.y);
	m_lenText->SetPosition(ivec2(ClipBarMeasure::m_leftOffset, ClipBarMeasure::m_lenToTop));

	m_btnPlay->OnPosit(aP.x, aP.y);
	m_btnPlay->SetPosition(ivec2(m_width - m_btnPlay->GetWidth() - ClipBarMeasure::m_rightOffset, ClipBarMeasure::m_lenToTop));

	m_editClipName->OnPosit(aP.x, aP.y);
	m_editClipName->SetPosition(ivec2(ClipBarMeasure::m_leftOffset, m_lenText->GetPositionY() + m_lenText->GetHeight() + ClipBarMeasure::m_interval));

	m_startText->OnPosit(aP.x, aP.y);
	m_startText->SetPosition(ivec2(ClipBarMeasure::m_leftOffset, m_editClipName->GetPositionY() + m_editClipName->GetHeight() + ClipBarMeasure::m_interval));
	
	m_endText->OnPosit(aP.x, aP.y);
	m_endText->SetPosition(ivec2(m_width - m_endText->GetWidth() - ClipBarMeasure::m_rightOffset, m_startText->GetPositionY()));
	
	m_btnStart->OnPosit(aP.x, aP.y);
	m_btnStart->SetPosition(ivec2(ClipBarMeasure::m_leftOffset - ClipBarMeasure::m_slideLen / 2.0, GetHeight() - ClipBarMeasure::m_slideLen - ClipBarMeasure::m_slideToBottom));
	m_minPositionX = m_btnStart->GetPositionX();
	if(m_length != 0)
		m_btnStart->AddPosition(m_start * (ClipBarMeasure::m_axisLen / m_length), 0.0);
	
	m_btnEnd->OnPosit(aP.x, aP.y);
	m_btnEnd->SetPosition(ivec2(ClipBarMeasure::m_leftOffset + ClipBarMeasure::m_axisLen - ClipBarMeasure::m_slideLen / 2.0, m_btnStart->GetPositionY()));
	m_maxPositionX = m_btnEnd->GetPositionX();
	if(m_length != 0)
		m_btnEnd->AddPosition(ClipBarMeasure::m_axisLen - m_end * (ClipBarMeasure::m_axisLen / m_length), 0.0);

	m_axisPosition.x = ClipBarMeasure::m_leftOffset;
	m_axisPosition.y = m_startText->GetPositionY() + m_startText->GetHeight() + ClipBarMeasure::m_axisToStart;

	m_btnMinus->OnPosit(aP.x, aP.y);
	m_btnMinus->SetPosition(ivec2(m_axisPosition.x - m_btnMinus->GetWidth() - 3, m_axisPosition.y - 7));

	m_btnAdd->OnPosit(aP.x, aP.y);
	m_btnAdd->SetPosition(ivec2(m_axisPosition.x + ClipBarMeasure::m_axisLen + 2, m_axisPosition.y - 7));
}

bool ClipBar::DispatchEvent(Event & ievent)
{
	if (!View::DispatchEvent(ievent))
		return false;

	//在本控件区域内
	if (m_btnStart->DispatchEvent(ievent))
	{
		m_isStart = true;
		return true;
	}

	if (m_btnEnd->DispatchEvent(ievent))
	{
		m_isStart = false;
		return true;
	}

	if (m_btnPlay->DispatchEvent(ievent))
		return true;

	if (m_editClipName->DispatchEvent(ievent))
		return true;

	if (m_btnAdd->DispatchEvent(ievent))
		return true;
	
	if (m_btnMinus->DispatchEvent(ievent))
		return true;

	static bool isDown = false;
	static ivec2 lastPosition;
	if (ievent.isMouseKeyDown())
	{
		isDown = true;
		lastPosition = ievent.m_mousePosition;
	}
	else if (ievent.isMouseKeyUp())
		isDown = false;

	if (isDown && ievent.m_mouseMotion == MouseMotion::MouseMove)
	{
		ivec2 curPostion = ievent.m_mousePosition;
		AddPosition(curPostion.x - lastPosition.x, curPostion.y - lastPosition.y);
		lastPosition = curPostion;
		ivec2 pP = m_parentView->GetAbsolutePosition();
		OnPosit(pP.x, pP.y);
	}
	return true;
}






//====================================================ListView===========================================
ListView::ListView(Activity *activity, string id, ivec2 position, int width, int height) : View(activity, id, position, width, height)
{
	
}

void ListView::AddItem(ListItem * item)
{
	if (item == nullptr)
		return;
	m_items.push_back(item);

	SetMeasureInvalidate();
	SetLayoutInvalidate();
	Invalidate(this);
}

void ListView::OnDraw(Tiny2D * paint)
{
	using Dragon::Math::MathUtility;
	using Dragon::Math::Rect;
	paint->SetDepth(Tiny2D::DrawDepth::NSecond);
	paint->EnableModel(Tiny2D::DrawModel::FORCE_DEPTH);
	paint->DrawRect(GetAbsolutePosition(), m_width, m_height, vec3(0.5, 0.2, 0.7));
	paint->EnableModel(Tiny2D::DrawModel::NORMAL_DEPTH);
	paint->SetDepth(Tiny2D::DrawDepth::NFirst);
	for (list<ListItem*>::iterator i = m_items.begin(); i != m_items.end(); i++)
	{
		//ListView绘制窗口与待绘制ListItem区域有交集，进行绘制		
		if(MathUtility::RectInterSect(Rect((*i)->GetPosition(), (*i)->GetWidth(), (*i)->GetHeight()), Rect(ivec2(0, 0), GetWidth(), GetHeight())))
			(*i)->OnDraw(paint);
	}
	paint->SetDepth(Tiny2D::DrawDepth::Normal);
}

void ListView::OnMeasure(int fatherWidth, int fatherHeight)
{
	View::OnMeasure(fatherWidth, fatherHeight);
	for (list<ListItem*>::iterator i = m_items.begin(); i != m_items.end(); i++)
		(*i)->OnMeasure(m_width - 2 * LRPadding, m_height);
}

void ListView::OnPosit(int fatherWidth, int fatherHeight)
{
	View::OnPosit(fatherWidth, fatherHeight);
	ivec2 aP = GetAbsolutePosition();
	int height = TBPadding;
	for (list<ListItem*>::iterator i = m_items.begin(); i != m_items.end(); i++)
	{
		(*i)->SetPosition(ivec2(LRPadding, height));
		(*i)->OnPosit(aP.x, aP.y);
		height += (*i)->GetHeight() + 2 * TBPadding;
	}
}

bool ListView::DispatchEvent(Event & ievent)
{
	if (!View::DispatchEvent(ievent))
		return false;

	for (list<ListItem*>::iterator i = m_items.begin(); i != m_items.end(); i++)
		if((*i)->DispatchEvent(ievent))
			return true;

	static bool isDown = false;
	static ivec2 lastPosition;
	if (ievent.isMouseKeyDown())
	{
		isDown = true;
		lastPosition = ievent.m_mousePosition;
	}
	else if (ievent.isMouseKeyUp())
		isDown = false;

	if (isDown && ievent.m_mouseMotion == MouseMotion::MouseMove)
	{
		ivec2 curPostion = ievent.m_mousePosition;
		AddPosition(curPostion.x - lastPosition.x, curPostion.y - lastPosition.y);
		lastPosition = curPostion;
		ivec2 pP = m_parentView->GetAbsolutePosition();
		OnPosit(pP.x, pP.y);
	}
	return true;
}







//====================================================ClipItem===========================================
ClipItem::ClipItem(Activity *activity, string clipName, float start, float end) : ListItem(activity)
{
	m_clipName = clipName;
	m_start = start;
	m_end = end;

	char str[20];
	m_texClip = new TextView(m_activity, m_id, vec2(0, 0), clipName);
	sprintf(str, "Start:%.1f", m_start);
	m_texStart = new TextView(m_activity, m_id, vec2(0, 0), str);
	sprintf(str, "End:%.1f", m_end);
	m_texEnd = new TextView(m_activity, m_id, vec2(0, 0), str);
}

void ClipItem::OnDraw(Tiny2D * paint)
{
	ivec2 aP = GetAbsolutePosition();
	paint->DrawRect(aP, m_width, m_height, vec3(0.45, 0.45, 0.45));
	m_texClip->OnDraw(paint);
	m_texStart->OnDraw(paint);
	m_texEnd->OnDraw(paint);
}

void ClipItem::OnMeasure(int fatherWidth, int fatherHeight)
{
	View::OnMeasure(fatherWidth, fatherHeight);

	m_texClip->OnMeasure(m_width, m_height);
	m_texStart->OnMeasure(m_width / 2.0, m_height);
	m_texEnd->OnMeasure(m_width / 2.0, m_height);

	m_height = TopPadding + m_texClip->GetHeight() + Interval + m_texStart->GetHeight() + BottomPadding;
}

void ClipItem::OnPosit(int fatherWidth, int fatherHeight)
{
	View::OnPosit(fatherWidth, fatherHeight);
	ivec2 aP = GetAbsolutePosition();
	m_texClip->OnPosit(aP.x, aP.y);
	m_texClip->SetPosition(ivec2(LeftPadding, TopPadding));
	int sumHeight = m_texClip->GetPositionY() + m_texClip->GetHeight() + Interval;
	m_texStart->OnPosit(aP.x, aP.y);
	m_texStart->SetPosition(ivec2(LeftPadding, sumHeight));
	m_texEnd->OnPosit(aP.x, aP.y);
	m_texEnd->SetPosition(ivec2(m_width - RightPadding - m_texEnd->GetWidth(), sumHeight));
}



//====================================================EditText===========================================
EditText::EditText(Activity * activity, const string id, ivec2 position, string text, int width, int height, TextAligin texAlign, vec3 color, int fontSize) : TextView(activity, id, position, text, width, height, texAlign, color, fontSize)
{
}

void EditText::OnDraw(Tiny2D * paint)
{
	ivec2 aP = GetAbsolutePosition();
	paint->DrawLine(aP, ivec2(aP.x + m_width, aP.y));
	paint->DrawLine(ivec2(aP.x + m_width, aP.y), ivec2(aP.x + m_width, aP.y + m_height));
	paint->DrawLine(aP, ivec2(aP.x, aP.y + m_height));
	paint->DrawLine(ivec2(aP.x, aP.y + m_height), ivec2(aP.x + m_width, aP.y + m_height));
	TextView::OnDraw(paint);
}

bool EditText::DispatchEvent(Event & ievent)
{
	if (ievent.m_hasCharMsg)
	{
		SetText(m_text.append(1, ievent.m_codePoint));
		return true;
	}
	else if (ievent.m_hasKeyMsg && (ievent.m_keyMotion == KeyMotion::KeyDown || ievent.m_keyMotion == KeyMotion::KeyRepeat))
	{
		if (ievent.m_keyCode == KEY_BACK)
		{
			string str = GetText();
			if (str.size() == 0)
				return true;
			str.erase(--str.end());
			SetText(str);
		}
	}

	if (!View::DispatchEvent(ievent))
		return false;
	
	return true;

}

void EditText::OnMeasure(int fatherWidth, int fatherHeight)
{
	TextView::OnMeasure(fatherWidth, fatherHeight);
	if (m_layoutWidth == View::Dimension::WRAP_CONTENT)
		m_width += LRPadding * 2;
	if (m_layoutHeight == View::Dimension::WRAP_CONTENT)
		m_height += TBPadding * 2;
}

void EditText::OnPosit(int fatherX, int fatherY)
{
	TextView::OnPosit(fatherX, fatherY);
	m_fontPosition.x = (m_width - m_texWidth) / 2.0;
	m_fontPosition.y = (m_height - m_texHeight) / 2.0;
}




bool ClipBar::ClipButtonListener::AddMinusButton(bool isAdd, const Event &e)
{
	if (e.m_mouseMotion == MouseMotion::LeftButtonUp)
	{
		if (m_clipBar->m_isStart)
			m_clipBar->AddStartValue(isAdd ? 1 : -1);
		else
			m_clipBar->AddEndValue(isAdd ? 1 : -1);
	}
	return true;
}

bool ClipBar::ClipButtonListener::PlayButton(View & view, const Event & e)
{
	static bool isPlay = false;
	if (e.m_mouseMotion == MouseMotion::LeftButtonUp)
	{
		if (!isPlay)
		{
			Button &button = dynamic_cast<Button&>(view);
			button.SetText("Pause");
			//播放片段
			AnimationController *controller = m_clipBar->GetAnimationController();
			if (controller != nullptr)
				controller->PlayClip(new AnimationClip(m_clipBar->GetStartValue(), m_clipBar->GetEndValue()));
		}
		else
		{
			Button &button = dynamic_cast<Button&>(view);
			button.SetText("Play");
			//播放片段
			AnimationController *controller = m_clipBar->GetAnimationController();
			if (controller != nullptr)
				controller->Pause();
		}
		isPlay = !isPlay;
	}
	return true;
}

bool ClipBar::ClipButtonListener::StartEndButton(View & view, const Event & e)
{
	using namespace std;
	static bool down = false;
	static ivec2 position;
	if (e.m_mouseMotion == MouseMotion::LeftButtonDown)
	{
		down = true;
		position = e.m_mousePosition;
	}
	else if (e.m_mouseMotion == MouseMotion::LeftButtonUp)
		down = false;
	if (down && e.m_mouseMotion == MouseMotion::MouseMove)
	{
		int viewX = view.GetPositionX();
		float deltaX = e.m_mousePosition.x - position.x;
		if (viewX + deltaX > m_clipBar->GetSlideMaxX() || viewX + deltaX < m_clipBar->GetSlideMinX())
			return true;
		float len = deltaX / ClipBarMeasure::m_axisLen * m_clipBar->GetLength();
		if ("ClipBar.StartButton" == view.GetViewID())
			m_clipBar->AddStartValue(len);
		else
			m_clipBar->AddEndValue(len);
		view.AddPosition(deltaX, 0);
		position = e.m_mousePosition;
	}
	return true;
}
