#include "LevelComponents.h"

HurtBox::HurtBox(Cappuccino::HitBox box, float dps)
	:_hurtBox(box), _damagePerSecond(dps)
{	}

GravLift::GravLift(Cappuccino::HitBox box, float lift, glm::vec3 dir)
	: _areaOfAffect(box), _liftPower(lift), _direction(dir)
{	}

DoorLoc::DoorLoc(Cappuccino::HitBox box, float rotation)
	: _exitBox(box), _rotation(rotation)
{}

Door::Door(float rotation, Cappuccino::HitBox hitbox, Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs)
	: GameObject(*SHADER, textures, meshs)
{	id = "Door";
	_rotation = rotation;
	_rigidBody._hitBoxes.push_back(hitbox); 
	// WHY CANT IT COLLIDE REEEEEEEEEEEE
	_rigidBody._moveable = true;
	_rigidBody._canTouch = true;
	_rigidBody.myType = "Door";

	setActive(false);
}

void Door::setHitbox(Cappuccino::HitBox hitbox)
{
	_rigidBody._hitBoxes.clear();
	_rigidBody._hitBoxes.push_back(Cappuccino::HitBox(glm::vec3(0.0f), hitbox._size, hitbox._rotationMatrix));
}

void Door::childUpdate(float dt)
{
}

Teleporter::Teleporter(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs)
	:GameObject(*SHADER,textures,meshs)
{
}

void Teleporter::childUpdate(float dt)
{	if (_currentDelay) {
		_currentDelay += dt;
		if (_currentDelay >= _tpDelay)
			_currentDelay = 0.0f;
	}
		
}
