
#ifndef MOVE_PHYSICS_H
	#define MOVE_PHYSICS_H

class MovePhysics
{
public:
	MovePhysics(float maxSpeed = 5.0f, float speedGain = 1.0f, float frictionRate = 0.95f, float gravityRate = 0.95, float gravityAccel = 2.0f);
	virtual ~MovePhysics();

	void FeedMovementInfo(bool falling);
	void FeedMovementInfo(float front, float side);
	void UpdateMovementParams();

	void SetMaxSpeed(float value);
	void SetSpeedGain(float value);
	void SetFrictionRate(float value);

	float
		front,
		side,
		front_speed,
		side_speed,
		vertical_speed,
		speed_gain,
		friction_rate,
		max_speed,
		gravity_rate,
		gravity_acceleration;
	bool falling;
};

#endif // !MOVE_PHYSICS_H