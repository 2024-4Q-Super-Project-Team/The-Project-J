#pragma once

namespace State
{
	// �޸��� �ִϸ��̼��� ����ȴٴ���. Player�� �̵��ӵ��� ���⸸ŭ ���� �����ִ���
	class Player_Run : public FSMState
	{
	public:
		_FSM_STATE_INIT(Player_Run);
	public:
		virtual void Start();
		virtual void Update();
	};


}