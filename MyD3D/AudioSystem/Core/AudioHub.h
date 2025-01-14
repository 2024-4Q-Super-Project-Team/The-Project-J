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
	static FMOD::System* mSystem;
public:
	static inline auto GetSystem() {
		if (CanUse) return mSystem;
		else Helper::HRT(E_FAIL, "Nullpointer reference from AudioSystem");
	}
};