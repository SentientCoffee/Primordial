#include "Bullet.h"

Bullet::Bullet(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs, const glm::vec3& transformPosition, const glm::vec3& origin, const float mass, bool gravity)
:GameObject(SHADER, textures, meshs, 1.0f), _bullet(transformPosition, origin, mass, gravity)
{
}

void Bullet::childUpdate(float dt)
{
	_bullet.update(dt);
}
