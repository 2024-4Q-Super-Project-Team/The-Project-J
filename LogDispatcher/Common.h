#pragma once

namespace Log
{
	struct LogPackit
	{
		std::string sendLog;
		std::chrono::system_clock::time_point recvTime;
	};

	enum LOG_EVENT
	{
		LOG_EVENT_PROCESS,
		LOG_EVENT_SHUTDOWN,
		LOG_EVENT_COUNT
	};
}
