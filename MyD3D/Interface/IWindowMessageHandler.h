#pragma once

class ViewportScene;

namespace Engine
{
	class IWindowMessageHandler
	{
	public:
		virtual void OnWindowMessage(ViewportScene* _pViewport, UINT _msg, WPARAM _wParam, LPARAM _lParam) = 0;
	};
}

