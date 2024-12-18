#pragma once
#include "Interface.h"
#include "Loger.h"

namespace Log
{
	using SystemClock = std::chrono::system_clock::time_point;
	void PushDateTime(std::ostringstream& _oss, const SystemClock& _clock);
	void PushElapsedTime(std::ostringstream& _oss, const SystemClock& _start, const SystemClock& _now);

	class LogDispatcher : public ILogDispatcher
	{
	public:
		explicit LogDispatcher();
		virtual ~LogDispatcher();
	public:
		virtual void Release() override;
		virtual bool CreateILoger(ILoger** _ppILoger, UINT _maxWeight, UINT _maxQueueSize) override;
		virtual bool CloseILoger(ILoger** _ppILoger) override;
	public:
		void UpdateQueue();
		void WriteLog(const std::ostringstream& _logMsg);
		
	public:
		std::vector<LogPackit>	mLogArr;

		std::thread				mWorker;
		std::ofstream			mLogfile;
		SystemClock				mStartTime;
		UINT					mNumLoger;
		Loger*					mLogers[MAX_LOGER_SIZE] {};

		std::mutex				mMutex;
		std::condition_variable mCondition;
		bool					mStopThread;
	private:
		void LogWorker();
	};
	
}