#pragma once
#include "Cappuccino/HitBox.h"

class HurtBox {
public:
	HurtBox(Cappuccino::HitBox box,float dps);
private:
	Cappuccino::HitBox _hurtBox{ glm::vec3(0),glm::vec3(0) };
	float _damagePerSecond = 0.0f;
};

class GravLift {
public:
	GravLift(Cappuccino::HitBox box, float lift, glm::vec3 dir);

private:
	Cappuccino::HitBox _areaOfAffect{ glm::vec3(0),glm::vec3(0) };
	float _liftPower = 0.0f;
	glm::vec3 _direction{ 0.0f,1.0f,0.0f };
};