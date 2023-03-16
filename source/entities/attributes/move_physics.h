
#ifndef MOVE_PHYSICS_H
	#define MOVE_PHYSICS_H

class MovePhysics
{
public:
	MovePhysics(float maxSpeed = 5.0f, float frictionRate = 0.95f, float gravityRate = 0.95, float gravityAccel = 0.5f);
	virtual ~MovePhysics();

	virtual void UpdateMoveParams(float front, float side, bool falling);

	void SetMaxSpeed(float value);
	void SetFrictionRate(float value);

	float
		front_speed,
		side_speed,
		vertical_speed,
		friction_rate,
		max_speed,
		gravity_rate,
		gravity_acceleration;
};

#endif // !MOVE_PHYSICS_H
