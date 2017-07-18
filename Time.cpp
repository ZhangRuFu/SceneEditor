#include "Time.h"

Time::Time(void)
{

	if (m_instance == nullptr)
		m_instance = this;
	m_deltaTime = 0;

}

void Time::Elapse(unsigned int time)
{
	m_deltaTime = time;
}

Time *Time::m_instance = nullptr;