#pragma once

namespace InputDevice
{
	class InputDeviceBase
	{
	public:
		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Reset() = 0;
	};
}