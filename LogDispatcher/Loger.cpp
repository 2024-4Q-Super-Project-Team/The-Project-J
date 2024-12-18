#include "pch.h"
#include "Loger.h"
#include "LogDispatcher.h"

namespace Log
{
	Loger::Loger(LogDispatcher* _dispatcher, UINT _id, UINT _MaxWeight, UINT _MaxQueueSize)
		: mDispatcher(_dispatcher), mLogerID(_id), mMaxQueueSize(_MaxQueueSize)
		, mCurWeight(0), mMaxWeight(_MaxWeight), mSendQueue()
	{
	}

	Loger::~Loger()
	{
		mDispatcher->UpdateQueue();
	}

	bool Loger::Receive(const std::string& _log, UINT _weight)
	{
		if (mSendQueue.size() >= mMaxQueueSize)
			return false;
		{
			LogPackit packit;
			packit.sendLog = _log;
			packit.recvTime = std::chrono::system_clock::now();
			mCurWeight += _weight;
			std::lock_guard<std::mutex> lock(mLogerMutex);
			mSendQueue.push(packit);
		}

		if (mCurWeight >= mMaxWeight)
		{
			Send();
		}

		return true;
	}

	void Loger::SetMaxWeight(UINT _maxWeight)
	{
		if (mCurWeight >= _maxWeight)
		{
			Send();
		}
		mMaxWeight = _maxWeight;
	}

	void Loger::SetMaxQueueSize(UINT _maxQueueSize)
	{
		if (mSendQueue.size() >= _maxQueueSize)
		{
			Send();
		}
		mMaxQueueSize = _maxQueueSize;
	}

	void Loger::Send()
	{
		mCurWeight = 0;
		mDispatcher->UpdateQueue();
	}

	std::queue<LogPackit> Loger::GetQueue()
	{
		std::lock_guard<std::mutex> lock(mLogerMutex);
		return std::move(mSendQueue);
	}
}