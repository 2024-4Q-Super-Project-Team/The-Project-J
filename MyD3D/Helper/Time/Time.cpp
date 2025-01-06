#include "pch.h"
#include "Time.h"

LARGE_INTEGER Time::m_preTime = { 0,0 };
LARGE_INTEGER Time::m_curTime = { 0,0 };
LARGE_INTEGER Time::m_frequency = { 0,0 };
int			  Time::m_frameCount = 0;
float		  Time::m_frameTime = 0.f;
float		  Time::m_elapsedTime = 0.f;
int			  Time::m_fps = 0;
bool		  Time::m_freezeTime = false;
float         Time::m_maximumDeltatime = 0.5f;

float		  Time::m_deltaTime = 0.f;
float		  Time::m_timeScale = 1.f;

void Time::Initialize()
{
	QueryPerformanceCounter(&m_preTime); // CPU Å¬·°
	QueryPerformanceFrequency(&m_frequency);
}
void Time::Update()
{
	QueryPerformanceCounter(&m_curTime);
	m_deltaTime =
		(float)(m_curTime.QuadPart - m_preTime.QuadPart)
		/ (float)(m_frequency.QuadPart);

	m_preTime = m_curTime;
	m_frameTime += m_deltaTime;
	m_elapsedTime += m_deltaTime;
	m_frameCount++;

	if (m_deltaTime >= m_maximumDeltatime) m_deltaTime = m_maximumDeltatime;
	if (m_freezeTime) m_deltaTime = 0;
	if (m_frameTime >= 1.f)
	{
		m_fps = (int)(m_frameCount / m_frameTime);
		m_frameTime = 0.f;
		m_frameCount = 0;
	}
}