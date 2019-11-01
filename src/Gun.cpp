#include "Gun.h"
#include "Commando.h"
#include "Cappuccino/SoundSystem.h"

Gun::Gun(const Cappuccino::Shader& SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string weapon, const float damage, const float firerate, const int ammo)
	:GameObject(SHADER, textures, meshes, 1.0f), _weapon(weapon), _damage(damage), _firerate(firerate), _ammo(ammo)
{
}

void Gun::setDelay(float dt)
{
	_delay -= dt;
}

bool Gun::getFire()
{
	if (_delay <= 0.0f)
	{
		_delay = _firerate;
		return true;
	}
	else
		return false;
}

void Gun::addBullets(Bullet* bullet)
{
	int indexMax = bullet->getLife() / _firerate + 1;
	for (unsigned i = 0; i < indexMax; i++)
	{
		Bullet* temp = new Bullet(bullet->getShader(), bullet->getTextures(), bullet->getMeshes(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		temp->_transform.scale(glm::vec3(1.0f), 0.01f);
		_bullets.push_back(temp);
	}
}

void Gun::shoot(glm::vec3& camera, glm::vec3& pos)
{
	if (!(_ammoCount >= _ammo))
	{
		setDir(camera);
		_dirVec = glm::normalize(_dirVec);

		_bullets[_index]->_rigidBody.setVelocity(_dirVec * 1.5f);
		_bullets[_index]->_rigidBody._position = pos;

		_bullets[_index]->setActive(true);
		_index++;
		_ammoCount++;
		if (_index >= _bullets[_index]->getLife() / _firerate)
			_index = 0;
	}
}

void Gun::childUpdate(float dt)
{

}

void Gun::setShootSound(const std::string& path, const std::string& groupName)
{
	soundHandle = Cappuccino::SoundSystem::load2DSound(path);
	groupHandle = Cappuccino::SoundSystem::createChannelGroup(groupName);
}
