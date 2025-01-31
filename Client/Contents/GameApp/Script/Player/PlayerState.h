#pragma once

namespace State
{
	// 달리기 애니메이션이 재생된다던가. Player를 이동속도에 방향만큼 힘을 가해주던가
	class Player_Run : public FSMState
	{
	public:
		_FSM_STATE_INIT(Player_Run);
	public:
		virtual void Start();
		virtual void Update();
	};


}