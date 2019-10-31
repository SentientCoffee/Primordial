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
	for (unsigned i = 0; i < _ammo; i++)
		_bullets.push_back(bullet);
}

void Gun::shoot(glm::vec3 camera, glm::vec3 pos)
{
	if (!(index >= _bullets.size()))
	{
		setDir(camera);
		dirVec = glm::normalize(dirVec);

		_bullets[index]->_rigidBody.setVelocity(dirVec);
		_bullets[index]->_rigidBody._position = pos;

		_bullets[index]->setActive(true);
		index++;

		Cappuccino::SoundSystem::playSound2D(soundHandle, groupHandle,Cappuccino::SoundSystem::ChannelType::SoundEffect);

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
