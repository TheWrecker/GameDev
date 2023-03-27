
#include <cmath>

#include "move_physics.h"

constexpr float SPEED_INFO_MAX = 1.0f;
constexpr float MAX_FALL_SPEED = 300.0f;

MovePhysics::MovePhysics(float maxSpeed, float speedGain, float frictionRate, float gravityRate, float gravityAccel)
    :max_speed(maxSpeed), speed_gain(speedGain),friction_rate(frictionRate), front_speed(0.0f), side_speed(0.0f),
    vertical_speed(0.0f), gravity_rate(gravityRate), gravity_acceleration(gravityAccel), front(0.0f), side(0.0f),
    vertical(0.0f), falling(false)
{
}

MovePhysics::~MovePhysics()
{
}

void MovePhysics::FeedMovementInfo(bool fallingInfo)
{
    falling = fallingInfo;
}

void MovePhysics::FeedMovementInfo(float frontInfo, float sideInfo, float verticalInfo)
{
    //TODO: remove limiters for boosts/throws/pushes?
    front = fmax(-SPEED_INFO_MAX, fmin(frontInfo, SPEED_INFO_MAX));
    side = fmax(-SPEED_INFO_MAX, fmin(sideInfo, SPEED_INFO_MAX));
    vertical = verticalInfo;
    UpdateMovementParams();
}

void MovePhysics::UpdateMovementParams()
{
    if (front > 0.0f)
    {
        front_speed + (front * speed_gain) > max_speed ? front_speed = max_speed : front_speed += (front * speed_gain);
    }
    else if (front < 0.0f)
    {
        abs(front_speed + (front * speed_gain)) > max_speed ? front_speed = -max_speed : front_speed += (front * speed_gain);
    }
    else
    {
        if (abs(front_speed) < 0.1f)
            front_speed = 0.0;
        else
            front_speed *= friction_rate;
    }

    if (side > 0.0f)
    {
        side_speed + (side * speed_gain) > max_speed ? side_speed = max_speed : side_speed += (side * speed_gain);
    }
    else if (side < 0.0f)
    {
        abs(side_speed + (side * speed_gain)) > max_speed ? side_speed = -max_speed : side_speed += (side * speed_gain);
    }
    else
    {
        if (abs(side_speed) < 0.1f)
            side_speed = 0.0;
        else
            side_speed *= friction_rate;
    }

    if (vertical > 0.0f)
    {
        vertical_speed += vertical;
    }
    else if (vertical < 0.0f)
    {
        abs(vertical_speed + (vertical)) > MAX_FALL_SPEED ? vertical_speed = -MAX_FALL_SPEED : vertical_speed += vertical;
    }
    else
    {
        if (vertical_speed > 0.5f)
            vertical_speed *= gravity_rate;
        else if (vertical_speed > 0.0f)
            vertical_speed = 0.0f;
        else
        {
            if (falling)
                abs(vertical_speed - gravity_acceleration) > MAX_FALL_SPEED ? vertical_speed = -MAX_FALL_SPEED : vertical_speed -= gravity_acceleration;
            else
                vertical_speed = 0.0f;
        }
    }
}

void MovePhysics::SetMaxSpeed(float value)
{
    max_speed = abs(value);
}

void MovePhysics::SetSpeedGain(float value)
{
    max_speed = abs(value);
}

void MovePhysics::SetFrictionRate(float value)
{
    max_speed = abs(value);
}
