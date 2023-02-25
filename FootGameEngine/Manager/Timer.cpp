#include "GamePch.h"
#include "Timer.h"

std::shared_ptr<GameEngineSpace::Timer> GameEngineSpace::Timer::instance = nullptr;

namespace GameEngineSpace
{

	std::shared_ptr<Timer> Timer::GetInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<Timer>();

		return instance;
	}

	void Timer::Init()
	{
		secondsPerCount = 0.0f;
		deltaTime = 0.0f;
		baseTime = 0;
		stopTime = 0;
		pausedTime = 0;
		prevTime = 0;
		currTime = 0;
		isStopped = false;

		int64 _countsPerSec;

		QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&_countsPerSec));
		secondsPerCount = 1.0f / static_cast<float>(_countsPerSec);

		printf("Timer Init Clear\n");

	}

	void Timer::Release()
	{

	}

	float Timer::TotalTime() const
	{
		if (isStopped == true)
			return static_cast<float>(((stopTime - pausedTime) - baseTime) * secondsPerCount);
			// 현재 시간에서 시작 시간을 빼면. 그 사이에 일시정지된 시간이 있을 수 있다.
		else		
			return static_cast<float>(((currTime - pausedTime) - baseTime) * secondsPerCount);		
	}

	float Timer::DeltaTime() const
	{
		return static_cast<float>(deltaTime);

	}

	void Timer::Reset()
	{
		int64 _currTime;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_currTime));

		baseTime = _currTime;
		prevTime = 0;
		stopTime = 0;
		isStopped = false;
	}

	void Timer::Start()
	{
		int64 _startTime;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_startTime));

		if (isStopped == true)
		{
			pausedTime += (_startTime - stopTime);

			prevTime = _startTime;

			stopTime = 0;
			isStopped = false;
		}
	}


	void Timer::Stop()
	{
		if(isStopped != true)
		{
			int64 _currTime;
			QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_currTime));

			// 중단 시간을 저장 후 중단 되었다고 알림.
			stopTime = _currTime;
			isStopped = true;
		}
	}

	void Timer::Tick()
	{
		if (isStopped == true)
		{
			deltaTime = 0.0f;
			return;
		}

		// 이번 프레임에 시간을 얻고
		int64 _currTime;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_currTime));
		currTime = _currTime;

		// 델타 타임을 구한다..
		deltaTime = static_cast<float>(currTime - prevTime) * secondsPerCount;

		// 다음 프레임을 준비하고.
		prevTime = currTime;

		// 델타 타임이 음수가 되지 않게끔 한다.
		if (deltaTime < 0.0f)
			deltaTime = 0.0f;
	}
}
