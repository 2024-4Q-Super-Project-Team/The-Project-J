#pragma once

namespace Log
{
	class ILogDispatcher;
	class ILoger;

	// IO작업 스레드 생성 및 관리, Loger 생성 및 관리
	class ILogDispatcher
	{
	public:
		explicit ILogDispatcher() = default;
		virtual ~ILogDispatcher() = default;
		ILogDispatcher(ILogDispatcher&) = delete;
		ILogDispatcher& operator=(const ILogDispatcher&) = delete;
		ILogDispatcher(ILogDispatcher&&) noexcept = default;
		ILogDispatcher& operator=(ILogDispatcher&&) noexcept = default;
	public:
		virtual void Release() = 0;
		virtual bool CreateILoger(ILoger** _ppILoger, UINT _maxWeight, UINT _maxQueueSize) = 0;
		virtual bool CloseILoger(ILoger** _ppILoger) = 0;
	};
	// Log메세지를 받아 Dispathcer에 보내주는 클래스.
	class ILoger
	{
	public:
		explicit ILoger() = default;
		virtual ~ILoger() = default;
		ILoger(ILoger&) = delete;
		ILoger& operator=(const ILoger&) = delete;
		ILoger(ILoger&&) noexcept = default;
		ILoger& operator=(ILoger&&) noexcept = default;
	public:
		virtual bool Receive(const std::string& _log, UINT _weight) = 0;
		virtual void Send() = 0;
		virtual void SetMaxWeight(UINT _maxWeight) = 0;
		virtual void SetMaxQueueSize(UINT _maxQueueSize) = 0;
	};
}