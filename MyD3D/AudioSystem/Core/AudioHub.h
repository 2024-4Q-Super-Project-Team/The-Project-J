#pragma once

// Audio 및 Group 등 생성 관련 클래스
class AudioHub
{
public:
	explicit AudioHub();
	virtual ~AudioHub();
public:
	static BOOL Initialize();
	static void Finalization();
public:
	static void Update();
private:
	static BOOL CanUse;
	static FMOD::System* mCoreSystem;
	static FMOD::Studio::System* mSystem;
public:
	static inline auto GetCoreSystem() {
		if (CanUse) return mCoreSystem;
		else Helper::HRT(E_FAIL, "Nullpointer reference from AudioSystem");
	}
	static inline auto GetNumDriver() {
		if (CanUse)
		{
			int numDrivers = 0;
			mCoreSystem->getNumDrivers(&numDrivers);
			return numDrivers;
		}
	}
};