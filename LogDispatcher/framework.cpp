#include "pch.h"
#include "framework.h"
#include "LogDispatcher.h"

// TODO: 라이브러리 함수의 예제입니다.
namespace Log
{
	bool CreateLogDispatcher(ILogDispatcher** _ppILogDispatcher)
	{
		LogDispatcher* instance;
		instance = new LogDispatcher();
		if (instance)
		{
			(*_ppILogDispatcher) = instance;
			return true;
		}
		return false;
	}
}