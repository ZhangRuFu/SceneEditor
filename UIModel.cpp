#include "UIModel.h"
#include "Activity.h"

void UIModel::AddActivity(Activity * activity)
{
	for (list<Activity*>::const_iterator i = m_activities.cbegin(); i != m_activities.cend(); i++)
		if ((*i) == activity)
			//÷ÿ∏¥
			return;
	m_activities.push_back(activity);
}

void UIModel::ActiveActivity(string id)
{
	for (list<Activity*>::const_iterator i = m_activities.cbegin(); i != m_activities.cend(); i++)
		if ((*i)->GetViewID() == id)
		{
			m_activities.insert(m_activities.begin(), (*i));
			m_activities.erase(i);
			return;
		}
}

void UIModel::DeleteActivity(string id)
{
	for (list<Activity*>::const_iterator i = m_activities.cbegin(); i != m_activities.cend(); i++)
		if ((*i)->GetViewID() == id)
		{
			m_activities.erase(i);
			return;
		}
}
