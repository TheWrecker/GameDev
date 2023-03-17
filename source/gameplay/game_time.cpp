
#include "../core/sys_ticker.h"

#include "game_time.h"

GameTime::GameTime(SystemTicker* ticker)
    :ticker(ticker), speed(1.0f), game_time(0.0f), real_time(0.0f), paused(true), realtime_container(), gametime_container()
{
}

GameTime::~GameTime()
{
}

void GameTime::Update()
{
    if (!paused)
    {
        auto _game_diff = (ticker->GetLastTickDuration() * speed);
        game_time += _game_diff;
        auto _game_ms = static_cast<unsigned int>(_game_diff * 1000);

        for (auto& _element : gametime_container)
        {
            _element.current += _game_ms;
            if (_element.current > _element.call_limit)
            {
                _element.current -= _element.call_limit;
                _element.func();
            }
        }
    }

    auto _real_diff = ticker->GetLastTickDuration();
    real_time += (_real_diff);
    auto _real_ms = static_cast<unsigned int>(_real_diff * 1000);

    for (auto& _element : realtime_container)
    {
        _element.current += _real_ms;
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

PeriodicTask GameTime::RegisterFunction(Callback func, unsigned int interval, bool realTime)
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
