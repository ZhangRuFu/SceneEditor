#pragma once
/*
*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：Time
*	描　　述：Time类
*
*/

class Time
{
private:
	unsigned int m_deltaTime;
	static Time *m_instance;

	
public:
	Time(void);

	void Elapse(unsigned int time);

	static unsigned int GetDeltaTime(void) { return m_instance->m_deltaTime; }
};