#pragma once

class Time
{
	static LARGE_INTEGER m_preTime;
	static LARGE_INTEGER m_curTime;
	static LARGE_INTEGER m_frequency;
	static int			 m_frameCount;
	static float		 m_frameTime;
	static float		 m_elapsedTime;
	static int			 m_fps;
	static bool		     m_freezeTime;
	
	static float		 m_timeScale;
	static float         m_maximumDeltatime;
	static float         m_deltaTime;
public:
	static void			 Initialize();
	static void			 Update();
public:
	static float		 GetScaledDeltaTime() { return m_deltaTime * m_timeScale; }
	static float		 GetUnScaledDeltaTime() { return m_deltaTime; }
	static float         GetTimeScale() { return m_timeScale; }
	static float		 GetMaximumDeltaTime() { return m_maximumDeltatime; }
	static bool			 GetFreezeTime() { return m_freezeTime; }
	static float		 GetElapsedTime() { return m_elapsedTime; }
	static int			 GetFps() { return m_fps; }

	static void			 SetTimeScale(float _val) { m_timeScale = _val; }
	static void			 SetFreezeTime(bool _val) { m_freezeTime = _val; }
	static void			 SetMaximumDeltaTime(float _maximumDt) { m_maximumDeltatime = _maximumDt; }
};

