#pragma once
#include <list>
#include <string>


using std::list;
using std::string;

class Activity;
class Tiny2D;

class UIModel
{
private:
	list<Activity*> m_activities;

public:
	virtual void AddActivity(Activity *activity);

	void ActiveActivity(string id);

	void DeleteActivity(string id);

	Activity* GetActive(void) { return (m_activities.size() > 0 ? *m_activities.begin() : nullptr); }
};