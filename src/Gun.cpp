#include "Gun.h"

Gun::Gun(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs, std::string& weapon, float& damage, float& firerate)
	:GameObject(SHADER, textures, meshs, 1.0f)
{
	_weapon = weapon;
	_damage = damage;
	_firerate = firerate;
	_fire = false;
}

bool Gun::getFire(float dt)
{
	return dt >= _firerate;
}

void Gun::addBullets(Bullet bullet)
{
	_bullets.push_back(bullet);
}