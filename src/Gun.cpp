#include "Gun.h"

Gun::Gun(const Cappuccino::Shader& SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string weapon, const float damage, const float firerate, const int ammo)
	:GameObject(SHADER, textures, meshes, 1.0f), _weapon(weapon), _damage(damage), _firerate(firerate), _ammo(ammo), _fire(false)
{
}

bool Gun::getFire(float dt)
{
	return dt >= _firerate;
}

void Gun::addBullets(glm::vec3& pos, glm::vec3& direction)
{
	Bullet bullet = Bullet(Cappuccino::Shader{ "font.vert","font.frag" }, std::vector<Cappuccino::Texture*>{}, std::vector<Cappuccino::Mesh*>{new Cappuccino::Mesh("Assets/Meshes/NUTtest.obj")}, pos, direction);
	bullet.setActive(true);
	_bullets.push_back(bullet);

}

void Gun::childUpdate(float dt)
{
	for (unsigned int i = 0; i < _bullets.size(); i++)
	{
		_bullets[i].childUpdate(dt);
		if (_bullets[i].getLife())
			_bullets.erase(_bullets.begin() + i);
	}
}
