#pragma once
#include "Cappuccino/HitBox.h"

struct HurtBox {
public:
	HurtBox() {};
	HurtBox(Cappuccino::HitBox box,float dps);
	Cappuccino::HitBox _hurtBox{ glm::vec3(0),glm::vec3(0) };
	float _damagePerSecond = 0.0f;
};

struct GravLift {
public:
	GravLift() {};
	GravLift(Cappuccino::HitBox box, float lift, glm::vec3 dir = glm::vec3(0.0f,1.0f,0.0f));
	Cappuccino::HitBox _areaOfAffect{ glm::vec3(0),glm::vec3(0) };
	float _liftPower = 110.0f;
	glm::vec3 _direction{ 0.0f,1.0f,0.0f };
};

struct Door {
public:
	Door() {};
	Door(Cappuccino::HitBox box, float rotation);
	float _rotation = 0.0f;
	Cappuccino::HitBox _exitBox = Cappuccino::HitBox(glm::vec3(0.0f), glm::vec3(1.0f));
};

struct Tele {
public:
	Tele() {};
	Tele(Cappuccino::HitBox box);
	Cappuccino::HitBox _porter = Cappuccino::HitBox(glm::vec3(0.0f), glm::vec3(1.0f));

};