#pragma once

#include <math.h>

class PositionClass
{
public:
	PositionClass();
	PositionClass(const PositionClass&);
	~PositionClass();

	void SetFrameTime(float);
	void GetRotation(float&, float&);

	void TurnLeft(bool);
	void TurnRight(bool);

private:
	float m_frameTime;
	float m_rotationX, m_rotationY;
	float m_leftTurnSpeed, m_rightTurnSpeed, m_upTurnSpeed, m_downTurnSpeed;
};

