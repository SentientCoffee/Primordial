#include "Bullet.h"

Bullet::Bullet() : GameObject(Cappuccino::Shader{}, {}, {})
{
}

Bullet::Bullet(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs, glm::vec3& pos, glm::vec3&direction)
:GameObject(SHADER, textures, meshs, 1.0f)
{
	_rigidBody._position = pos;
	_rigidBody.setVelocity(direction);
	_rigidBody.setGrav(false);
	_life = 5.0f;
	_rigidBody._canTouch = false;
}

Bullet::Bullet(const Bullet&& b) : GameObject(b._shader,b._textures, b._meshes)
{
	_rigidBody._position = b._rigidBody._position;
	_rigidBody.setGrav(false);
	_rigidBody.setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
	_life = b._life;
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
}
