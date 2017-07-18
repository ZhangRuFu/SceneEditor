//#include <GLM\gtx\transform.hpp>
//#include "UIDrawer.h"
//#include "UI.h"
//#include "RenderSystem.h"
//#include "Tiny2D.h"
//
//UIDrawer::UIDrawer(void) {}
//
//ButtonDrawer::ButtonDrawer(const Button *button)
//{
//	m_button = button;
//	m_textDrawer = dynamic_cast<TextViewDrawer*>(button->GetTextView()->GetDrawer());
//}
//
//void ButtonDrawer::Draw()
//{
//	
//	glm::vec3 position = glm::vec3(m_button->GetAbsolutePosition(), 0.0f);
//	glm::ivec2 dimension = m_button->GetDimension();
//
//	int buttonState = m_button->GetButtonState();
//
//	static glm::vec3 buttonColor = vec3(0.0f, 0.0f, 0.9f);
//	static glm::vec3 offset = vec3(0.1f, 0.1f, 0.1f);
//	vec3 color = buttonColor;
//	if (buttonState == ButtonState::Focus)
//		color += offset;
//	else if (buttonState == ButtonState::Clicked)
//		color -= offset;
//	
//	DrawRect(position, dimension.x, dimension.y, color);
//	m_textDrawer->Draw();
//	
//}
//
//
//TextViewDrawer::TextViewDrawer(const TextView * texView)
//{
//	m_texView = texView;
//}
//
//
//void TextViewDrawer::Draw(void)
//{
//	DrawText(m_texView->GetText(), m_texView->GetAbsolutePosition(), m_texView->GetFontSize(), vec3(1.0f, 1.0f, 1.0f));
//}
//
//ClipBarDrawer::ClipBarDrawer(const ClipBar * clipBar)
//{
//	m_clipBar = clipBar;
//	m_startText = dynamic_cast<TextViewDrawer*>(clipBar->GetStartTextView()->GetDrawer());
//	m_endText = dynamic_cast<TextViewDrawer*>(clipBar->GetEndTextView()->GetDrawer());
//	m_lenText = dynamic_cast<TextViewDrawer*>(clipBar->GetLengthTextView()->GetDrawer());
//	m_startButton = dynamic_cast<ButtonDrawer*>(clipBar->GetStartButton()->GetDrawer());
//	m_endButton = dynamic_cast<ButtonDrawer*>(clipBar->GetEndButton()->GetDrawer());
//	m_axisPosition.x = ClipBarMeasure::m_leftOffset;
//	m_axisPosition.y = clipBar->GetStartTextView()->GetPositionY() + clipBar->GetStartTextView()->GetHeight() + ClipBarMeasure::m_axisToStart;
//}
//
//void ClipBarDrawer::Draw(void)
//{
//	ivec2 rt;
//	ivec2 dimension;
//	rt = m_clipBar->GetAbsolutePosition();
//	dimension = m_clipBar->GetDimension();
//	DrawRoundRect(rt, dimension.x, dimension.y, 15 ,vec3(0.7f, 0.1f, 0.2f));
//	rt.x += m_axisPosition.x;
//	rt.y += m_axisPosition.y;
//	DrawLine(vec2(rt.x, rt.y), vec2(rt.x + ClipBarMeasure::m_axisLen, rt.y));
//	DrawLine(vec2(rt.x, rt.y - 5), vec2(rt.x, rt.y + 5));
//	DrawLine(vec2(rt.x + ClipBarMeasure::m_axisLen, rt.y - 5), vec2(rt.x + ClipBarMeasure::m_axisLen, rt.y + 5));
//	m_startText->Draw();
//	m_endText->Draw();
//	m_lenText->Draw();
//	m_startButton->Draw();
//	m_endButton->Draw();
//}
//
//ListViewDrawer::ListViewDrawer(const ListView * listView)
//{
//	m_listView = listView;
//	list<ListItem*> items = listView->GetItems();
//	for (list<ListItem*>::const_iterator i = items.cbegin(); i != items.cend(); i++)
//		m_itemDrawers.push_back((*i)->GetDrawer());
//}
//
//void ListViewDrawer::Draw()
//{
//	ivec2 dimension = m_listView->GetDimension();
//	ivec2 rt = m_listView->GetAbsolutePosition();
//	DrawRect(rt, dimension.x, dimension.y, vec3(0.75, 1.0, 0.24));
//	
//	list<ListItem*> items = m_listView->GetItems();
//	for (list<ListItem*>::const_iterator i = items.cbegin(); i != items.cend(); i++)
//		(*i)->GetDrawer()->Draw();
//	/*for (list<UIDrawer*>::const_iterator i = m_itemDrawers.cbegin(); i != m_itemDrawers.cend(); i++)
//		(*i)->Draw();*/
//}
//
//ClipItemDrawer::ClipItemDrawer(ClipItem * clipItem)
//{
//	m_clipItem = clipItem;
//	m_texClip = dynamic_cast<TextViewDrawer*>(m_clipItem->GetClipNameTextView()->GetDrawer());
//	m_texStart = dynamic_cast<TextViewDrawer*>(m_clipItem->GetStartTextView()->GetDrawer());
//	m_texEnd = dynamic_cast<TextViewDrawer*>(m_clipItem->GetEndTextView()->GetDrawer());
//}
//
//void ClipItemDrawer::Draw(void)
//{
//	ivec2 itemDimension;
//	itemDimension = m_clipItem->GetDimension();
//	ivec2 rt = m_clipItem->GetAbsolutePosition();
//	DrawRect(rt, itemDimension.x, itemDimension.y, vec3(0.5, 0.2, 0.5));
//	m_texClip->Draw();
//	m_texStart->Draw();
//	m_texEnd->Draw();
//}
