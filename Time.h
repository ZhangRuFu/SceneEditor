#pragma once
/*
*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������Time
*	�衡������Time��
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