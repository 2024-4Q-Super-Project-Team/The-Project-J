#pragma once
#include "Interface.h"

namespace Log
{
	class LogDispatcher;

	

	class Loger : public ILoger
	{
	public:
		explicit Loger(LogDispatcher* _dispatcher, UINT _id, UINT _MaxWeight, UINT _MaxQueueSize);
		virtual ~Loger();
	public:
		virtual bool Receive(const std::string& _log, UINT _weight) override;
		virtual void Send() override;
		virtual void SetMaxWeight(UINT _maxWeight) override;
		virtual void SetMaxQueueSize(UINT _maxQueueSize) override;
	public:
		std::queue<LogPackit> GetQueue();
	private:
		UINT mLogerID;
		UINT mMaxQueueSize;
		UINT mCurWeight;
		UINT mMaxWeight;
		std::queue<LogPackit> mSendQueue;
		std::mutex mLogerMutex;
		LogDispatcher* mDispatcher;
	};
}

