
#include "../core/sys_ticker.h"

#include "game_time.h"

GameTime::GameTime(SystemTicker* ticker)
    :ticker(ticker), speed(1.0f), game_time(0.0f), real_time(0.0f), paused(false), realtime_container(), gametime_container(), diff(0.0f)
{
}

GameTime::~GameTime()
{
}

void GameTime::Update()
{
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

void GameTime::SetGameSpeed(float value)
{
    speed = max(0.1f, min(value, 10.0f));
}

void GameTime::Pause()
{
    paused = true;
}

void GameTime::Resume()
{
    paused = false;
}

PeriodicTask GameTime::RegisterFunction(Callback func, float interval, bool realTime)
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

void GameTime::RemoveFunction(PeriodicTask target, bool realTime)
{
    if (realTime)
        realtime_container.erase(target);
    else
        realtime_container.erase(target);
}

float GameTime::GetGameSpeed()
{
    return speed;
}

float GameTime::GetGameTime()
{
    return game_time;
}

float GameTime::GetRealTime()
{
    return real_time;
}
