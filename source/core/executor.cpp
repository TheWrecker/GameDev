
#include <cassert>

#include "sys_ticker.h"
#include "supervisor.h"

#include "executor.h"

Executor::Executor(Supervisor* parent)
    :ticker(ticker), speed(1.0f), game_time(0.0f), real_time(0.0f), paused(false), realtime_container(),
    gametime_container(), diff(0.0f), supervisor(parent)
{
}

Executor::~Executor()
{
    supervisor->ExecutorDestroyed();
}

bool Executor::Initialize()
{
    ticker = supervisor->QueryService<SystemTicker*>("ticker");
    return (ticker != nullptr);
}

void Executor::Update()
{
    assert(ticker);

    diff = ticker->GetLastTickDuration();
    auto _ms = diff * 1000.0f;

    if (!paused)
    {
        game_time += diff * speed;
        for (auto& _element : gametime_container)
        {
            _element.current += _ms * speed;
            if (_element.current > _element.call_limit)
            {
                _element.current -= _element.call_limit;
                _element.func();
            }
        }
    }

    real_time += (diff);

    for (auto& _element : realtime_container)
    {
        _element.current += _ms;
        if (_element.current > _element.call_limit)
        {
            _element.current -= _element.call_limit;
            _element.func();
        }
    }
}

void Executor::SetGameSpeed(float value)
{
    speed = fmax(0.1f, fmin(value, 10.0f));
}

void Executor::Pause()
{
    paused = true;
}

void Executor::Resume()
{
    paused = false;
}

PeriodicTask Executor::RegisterPeriodicTask(Callback func, float interval, bool realTime)
{
    if (realTime)
    {
        realtime_container.emplace_back(func, interval);
        return realtime_container.cend()--;
    }
    else
    {
        gametime_container.emplace_back(func, interval);
        return realtime_container.cend()--;
    }
}

void Executor::RemovePeriodicTask(PeriodicTask target, bool realTime)
{
    if (realTime)
        realtime_container.erase(target);
    else
        realtime_container.erase(target);
}

float Executor::GetGameSpeed()
{
    return speed;
}

float Executor::GetGameTime()
{
    return game_time;
}

float Executor::GetRealTime()
{
    return real_time;
}
