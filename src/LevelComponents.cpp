#include "..\include\LevelComponents.h"

HurtBox::HurtBox(Cappuccino::HitBox box, float dps)
	:_hurtBox(box), _damagePerSecond(dps)
{	}

GravLift::GravLift(Cappuccino::HitBox box, float lift, glm::vec3 dir)
	: _areaOfAffect(box), _liftPower(lift), _direction(dir)
{	}

Door::Door(Cappuccino::HitBox box, float rotation)
	: _exitBox(box), _rotation(rotation)
{	}

Teleporter::Teleporter(Cappuccino::HitBox box, int key)
	: _areaOfAffect(box), _key(key)
{}
