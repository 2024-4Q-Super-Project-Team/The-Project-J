#pragma once
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
#include "Interface.h"

namespace Log
{
#define MAX_LOGER_SIZE 5

	bool CreateLogDispatcher(ILogDispatcher** _ppILogDispatcher);
}