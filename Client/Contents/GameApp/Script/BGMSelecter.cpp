#include "pch.h"
#include "BGMSelecter.h"

void BGMSelecter::Start()
{
	mStage01BGM = gameObject->GetComponent<AudioSource>();
	mStage02BGM = gameObject->GetComponent<AudioSource>();
	mStage03BGM = gameObject->GetComponent<AudioSource>();
}

void BGMSelecter::Update()
{
}