#include "Bullet.h"

Bullet::Bullet() : GameObject(Cappuccino::Shader{}, std::vector<Cappuccino::Texture*>{}, std::vector<Cappuccino::Mesh*>{})
{
}

Bullet::Bullet(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs, glm::vec3& pos, glm::vec3&direction)
:GameObject(SHADER, textures, meshs, 1.0f)
{
	_rigidBody._position = pos;
	_rigidBody.setVelocity(direction);
	_rigidBody.setGrav(false);
	_life = 5.0f;
}


float Bullet::getLife()
{
	return (_life);
}

bool Bullet::lifeState()
{
	return (_life <= 0.0f);
}

void Bullet::childUpdate(float dt)
{	_life -= dt;
	if (lifeState())
	{ 
		setActive(false);
		_life = 5.0f;
	}
	// check collisions here
}
