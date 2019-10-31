#include "Bullet.h"

Bullet::Bullet(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs, glm::vec3& pos, glm::vec3&direction)
:GameObject(SHADER, textures, meshs, 1.0f)
{
	_rigidBody._position = pos;
	_rigidBody.setAccel(direction);
	_life = 5.0f;
}

bool Bullet::getLife()
{
	return (_life <= 0.0f);
}

void Bullet::childUpdate(float dt)
{	_life -= dt;
	if (getLife())
	{ 
		setActive(false);
		_life = 5.0f;
	}
	// check collisions here
}
