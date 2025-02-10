#pragma once

class SpriteAnimScript :  public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(SpriteAnimScript);

public:
	virtual void Start() override;
	virtual void Update() override;
	void Play();

private:
	SpriteRenderer* mRenderer;

	int mFrameCount = 1;
	float mEffectTime = 0.5f;

	bool mIsPlaying = false;
	float mElapsedTime = 0.f;
	int mNowFrameIndex = 0;

};

