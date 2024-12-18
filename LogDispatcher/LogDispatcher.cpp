#include "pch.h"
#include "LogDispatcher.h"
#include "Loger.h"


namespace Log
{
	inline void PushDateTime(std::ostringstream& _oss, const SystemClock& _clock)
	{
		std::tm tm{};
		time_t clock = std::chrono::system_clock::to_time_t(_clock);
#ifdef _WIN32
		localtime_s(&tm, &clock); // Windows 환경
#else
		localtime_r(&clock, &tm); // POSIX 환경
#endif
		_oss << std::put_time(&tm, "%y%m%d_%H%M%S");
	}

	inline void PushElapsedTime(std::ostringstream& _oss, const SystemClock& _start, const SystemClock& _now)
	{
		auto elapsed = _now - _start;
		auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
		auto seconds = microseconds / 1'000'000;
		int hours = static_cast<int>(seconds / 3600);
		int minutes = static_cast<int>((seconds % 3600) / 60);
		int secs = static_cast<int>(seconds % 60);

		// 포맷팅된 문자열을 _oss에 추가
		_oss << std::setfill('0') << std::setw(2) << hours << ":"
			<< std::setw(2) << minutes << ":"
			<< std::setw(2) << secs << "."
			<< std::setw(6) << microseconds % 1'000'000;
	}

	LogDispatcher::LogDispatcher()
		: mStartTime()
		, mNumLoger(0)
		, mStopThread(false)
	{
		mStartTime = std::chrono::system_clock::now();
		mLogArr.reserve(100);

		std::filesystem::path curPath("./Logs");
		// Logs 폴더 유무 확인
		if (!std::filesystem::exists(curPath)) 
		{
			std::filesystem::create_directory(curPath);
		}

		{ // 파일 이름 결정 및 생성
			std::ostringstream oss;
			PushDateTime(oss, mStartTime);
#ifdef _DEBUG
			oss << "_Debug";
#else
			oss << "_Release";
#endif
			std::string fileName = curPath.string() + "//Log." + oss.str();

			// 로그 파일 생성
			mLogfile.open(fileName + ".log", std::ios::app);
		}

		{
			if (mLogfile.is_open())
			{
				std::ostringstream oss;
				PushElapsedTime(oss, mStartTime, std::chrono::system_clock::now());
#ifdef _DEBUG
				oss << " Created debug log file.\n";
#else
				oss << " Created release log file.\n";
#endif 
				WriteLog(std::move(oss));
			}
		}


		std::thread([this]() { LogWorker(); }).detach();
	}

	LogDispatcher::~LogDispatcher()
	{
	}
	void LogDispatcher::Release()
	{
		UpdateQueue();

		for (auto& v : mLogers)
		{
			if (v)
			{
				delete v;
				v = nullptr;
			}
		}

		if (mWorker.joinable())
		{
			mStopThread = true;
			mCondition.notify_all();
			mWorker.join();
		}

		if (mLogfile.is_open())
		{
			mLogfile.close();
		}

		delete this;
	}
	bool LogDispatcher::CreateILoger(ILoger** _ppILoger, UINT _maxWeight, UINT _maxQueueSize)
	{
		if (mNumLoger < MAX_LOGER_SIZE)
		{
			Loger* _instance;
			_instance = new Loger(this, mNumLoger, _maxWeight, _maxQueueSize);

			if (_instance)
			{
				mLogers[mNumLoger] = _instance;
				(*_ppILoger) = _instance;
				++mNumLoger;
				return true;
			}
		}
		return false;
	}

	bool LogDispatcher::CloseILoger(ILoger** _ppILoger)
	{
		if (nullptr == (*_ppILoger))
			return false;

		for (int i = 0; i < mNumLoger; ++i)
		{
			if (mLogers[i] == (*_ppILoger))
			{
				delete  mLogers[i];
				// 배열 요소를 왼쪽으로 밀어 순서 유지
				for (int j = i; j < mNumLoger - 1; ++j)
				{
					mLogers[j] = mLogers[j + 1];
				}
				mLogers[mNumLoger - 1] = nullptr;
				--mNumLoger; 
				return true;
			}
		}
		return false;
	}

	void LogDispatcher::UpdateQueue()
	{
		std::lock_guard<std::mutex> lock(mMutex);
		for (auto& loger : mLogers)
		{
			if (loger)
			{
				// 하나의 디스패쳐가 모든 로거에 접근해서 큐를 얻으려해서 발생하는 문제가 있었다...
				auto sendQueue = loger->GetQueue();
				while (!sendQueue.empty())
				{
					mLogArr.push_back(sendQueue.front());
					sendQueue.pop();
				}
			}
		}
		mCondition.notify_one();
	}

	void LogDispatcher::WriteLog(const std::ostringstream& _logMsg)  // const 참조로 변경
	{
		if (mLogfile.is_open())
		{
			mLogfile << _logMsg.str();
		}
	}


	void LogDispatcher::LogWorker()
	{
		while (true)
		{
			// 큐가 비어있지 않거나, 종료 변수가 true가 되었을 때까지 대기
			std::unique_lock lock(mMutex);
			mCondition.wait(lock, [this]() { return !mLogArr.empty() || mStopThread; });

			if (mStopThread) 
				break;

			// recvTime 기준으로 정렬
			std::sort(mLogArr.begin(), mLogArr.end(), [](const LogPackit& a, const LogPackit& b) {
				return a.recvTime < b.recvTime;
				});

			for (auto& [log, recvTime] : mLogArr)
			{
				std::ostringstream oss;
				PushElapsedTime(oss, mStartTime, recvTime);
				oss << " " << log << '\n';
				WriteLog(oss);
			}
			mLogArr.clear();
		}
		return;
	}
}
