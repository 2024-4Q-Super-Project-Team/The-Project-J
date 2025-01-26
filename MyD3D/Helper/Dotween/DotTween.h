#pragma once

#include "EasingFunction.h"
#include <functional> // 추가된 include

namespace Dotween
{
	enum StepAnimation
	{
		StepOnceForward,
		StepOnceBack,
		StepOncePingPong,
		StepLoopForward,
		StepLoopBack,
		StepLoopPingPong,

		StepAnimationEnd
	};

	template<typename T>
	class DotTween
	{
	public:
		DotTween(T& _Data, EasingEffect _EasingEffect, StepAnimation _StepAnimation = StepOnceForward)
			:Data(_Data), Function(EasingFunction[_EasingEffect]), Type(_StepAnimation)
		{
		}
		~DotTween() {};

		void	SetStartPoint(T _StartPoint) { StartPoint = _StartPoint; }
		void	SetEndPoint(T _EndPoint) { EndPoint = _EndPoint; }
		void	SetDuration(float	_Duration) { Duration = _Duration; }

		void	Update(const float& _DeltaTime)
		{
			(this->*StepAnimationFunction[Type])(_DeltaTime);
		}

	private:
		T& Data;			//	0 ~ 1(StartPoint ~ EndPoint)
		T							StartPoint;
		T							EndPoint;
		float						Duration;		// N초 동안
		float						CurTime = 0.f;
		std::function<float(float)>	Function;
		StepAnimation				Type;

	private:
		void OnceForward(const float& _DeltaTime)
		{
			if (CurTime > Duration)
				return;
			float CurStepTime;
			T CurStep;

			CurTime += _DeltaTime;
			CurStepTime = CurTime / Duration;
			CurStep = EndPoint - StartPoint;

			Data = StartPoint + CurStep * Function(CurStepTime);
		}
		void OnceBack(const float& _DeltaTime)
		{
			if (CurTime > Duration)
				return;
			float CurStepTime;
			T CurStep;

			CurTime += _DeltaTime;
			CurStepTime = 1.f - (CurTime / Duration);
			CurStep = EndPoint - StartPoint;

			Data = StartPoint + CurStep * Function(CurStepTime);
		}
		void OncePingPong(const float& _DeltaTime)
		{
			if (CurTime > Duration)
				return;
			float CurStepTime;
			T CurStep;

			CurTime += _DeltaTime;
			CurStepTime = (CurTime < (Duration / 2))
				? CurTime / (Duration / 2)
				: 1.f - ((CurTime - Duration / 2) / (Duration / 2));
			CurStep = EndPoint - StartPoint;

			Data = StartPoint + CurStep * Function(CurStepTime);
		}
		void LoopForward(const float& _DeltaTime)
		{
			if (CurTime > Duration)
				CurTime -= Duration;
			float CurStepTime;
			T CurStep;

			CurTime += _DeltaTime;
			CurStepTime = CurTime / Duration;
			CurStep = EndPoint - StartPoint;

			Data = StartPoint + CurStep * Function(CurStepTime);
		}
		void LoopBack(const float& _DeltaTime)
		{
			if (CurTime > Duration)
				CurTime -= Duration;
			float CurStepTime;
			T CurStep;

			CurTime += _DeltaTime;
			CurStepTime = 1.f - (CurTime / Duration);
			CurStep = EndPoint - StartPoint;

			Data = StartPoint + CurStep * Function(CurStepTime);
		}
		void LoopPingPong(const float& _DeltaTime)
		{
			if (CurTime > Duration)
				CurTime -= Duration;
			float CurStepTime;
			T CurStep;

			CurTime += _DeltaTime;
			CurStepTime = (CurTime < (Duration / 2))
				? CurTime / (Duration / 2)
				: 1.f - ((CurTime - Duration / 2) / (Duration / 2));
			CurStep = EndPoint - StartPoint;

			Data = StartPoint + CurStep * Function(CurStepTime);
		}

		static void (DotTween::* StepAnimationFunction[StepAnimation::StepAnimationEnd])(const float&);
	};

	// 정적 멤버 함수 포인터 배열 초기화
	template<typename T>
	void (DotTween<T>::* DotTween<T>::StepAnimationFunction[StepAnimation::StepAnimationEnd])(const float&) =
	{
		&DotTween<T>::OnceForward,
		&DotTween<T>::OnceBack,
		&DotTween<T>::OncePingPong,
		&DotTween<T>::LoopForward,
		&DotTween<T>::LoopBack,
		&DotTween<T>::LoopPingPong
	};
}
