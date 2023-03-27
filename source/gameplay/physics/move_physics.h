
#ifndef MOVE_PHYSICS_H
	#define MOVE_PHYSICS_H

	class MovePhysics
	{
	public:
		MovePhysics(float maxSpeed = 5.0f, float speedGain = 1.0f, float frictionRate = 0.98f, float gravityRate = 0.98f, float gravityAccel = 1.0f);
		virtual ~MovePhysics();

		void FeedMovementInfo(bool falling);
		void FeedMovementInfo(float front, float side, float vertical);
		void UpdateMovementParams();

		void SetMaxSpeed(float value);
		void SetSpeedGain(float value);
		void SetFrictionRate(float value);

		float
			front_speed,
			side_speed,
			vertical_speed,
			speed_gain,
			friction_rate,
			max_speed,
			gravity_rate,
			gravity_acceleration;
		bool falling;

	private:
		friend class PhysicsEngine;

		float
			front,
			side,
			vertical;
	};

#endif // !MOVE_PHYSICS_H
