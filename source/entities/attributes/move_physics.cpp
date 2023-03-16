
#include <cmath>

#include "move_physics.h"

MovePhysics::MovePhysics(float maxSpeed, float frictionRate, float gravityRate, float gravityAccel)
    :max_speed(maxSpeed), friction_rate(frictionRate), front_speed(0.0f), side_speed(0.0f), vertical_speed(0.0f),
    gravity_rate(gravityRate), gravity_acceleration(gravityAccel)
{
}

MovePhysics::~MovePhysics()
{
}

void MovePhysics::UpdateMoveParams(float front, float side, bool falling)
{
    if (front > 0.0f)
    {
        front_speed + front > max_speed ? front_speed = max_speed : front_speed += front;
    }
    else if (front < 0.0f)
    {
        abs(front_speed + front) > max_speed ? front_speed = -max_speed : front_speed += front;
    }
    else
    {
        front_speed *= friction_rate;
    }

    if (side > 0.0f)
    {
        side_speed + side > max_speed ? side_speed = max_speed : side_speed += side;
    }
    else if (side < 0.0f)
    {
        abs(side_speed + side) > max_speed ? side_speed = -max_speed : side_speed += side;
    }
    else
    {
        side_speed *= friction_rate;
    }

    if (falling)
    {
        if (vertical_speed > 0.1f)
            vertical_speed *= gravity_rate;
        else
        {
            abs(vertical_speed - gravity_acceleration) > max_speed ? vertical_speed = -max_speed : vertical_speed -= gravity_acceleration;
        }
    }
    else
    {
        vertical_speed = 0.0f;
    }
}

void MovePhysics::SetMaxSpeed(float value)
{
    max_speed = abs(value);
}

void MovePhysics::SetFrictionRate(float value)
{
    max_speed = abs(value);
}
