#include "Gun.h"
#include "Commando.h"
#include "Cappuccino/SoundSystem.h"

Gun::Gun(const Cappuccino::Shader& SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string weapon, const float damage, const float firerate, const int ammo)
	:GameObject(SHADER, textures, meshes, 1.0f), _weapon(weapon), _damage(damage), _firerate(firerate), _ammo(ammo)
{
	setActive(false);
}

Gun::Gun(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	: GameObject(*SHADER, textures, meshes)
{
	setActive(true);
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

void Gun::childUpdate(float dt)
{
}

void Gun::setShootSound(const std::string& path, const std::string& groupName)
{
	soundHandle = Cappuccino::SoundSystem::load2DSound(path);
	groupHandle = Cappuccino::SoundSystem::createChannelGroup(groupName);
}


AR::AR(const Cappuccino::Shader& SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string weapon, const float damage, const float firerate, const int ammo)
	:Gun(SHADER, textures, meshes, weapon, damage, firerate, ammo) 
{
	_offset = glm::vec3(0.0f, -0.05f, 0.0f);
}

Pistol::Pistol(const Cappuccino::Shader& SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string weapon, const float damage, const float firerate, const int ammo)
	: Gun(SHADER, textures, meshes, weapon, damage, firerate, ammo)
{
	_offset = glm::vec3(0.2f, -0.1f, 0.0f);
}

SG::SG(const Cappuccino::Shader& SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string weapon, const float damage, const float firerate, const int ammo, const int pellets)
	: Gun(SHADER, textures, meshes, weapon, damage, firerate, ammo), _pellets(pellets)
{
	_offset = glm::vec3(0.2f, -0.1f, 0.0f);
}

Crosshair::Crosshair(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	: Gun(SHADER, textures, meshes)
{
}

void AR::addBullets(Bullet* bullet)
{
	int indexMax = bullet->getLife() / _firerate + 1.0f;
	for (unsigned i = 0; i < indexMax; i++)
	{
		Bullet* temp = new Bullet(bullet->getShader(), bullet->getTextures(), bullet->getMeshes(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		temp->_transform.scale(glm::vec3(1.0f), 0.01f);
		_bullets.push_back(temp);
	}
}

void Pistol::addBullets(Bullet* bullet)
{
	int indexMax = bullet->getLife() / _firerate + 1.0f;
	for (unsigned i = 0; i < indexMax; i++)
	{
		Bullet* temp = new Bullet(bullet->getShader(), bullet->getTextures(), bullet->getMeshes(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		temp->_transform.scale(glm::vec3(1.0f), 0.01f);
		_bullets.push_back(temp);
	}
}

void SG::addBullets(Bullet* bullet)
{
	int indexMax = (bullet->getLife() / _firerate + 1.0f)*_pellets;
	for (unsigned i = 0; i < indexMax; i++)
	{
		Bullet* temp = new Bullet(bullet->getShader(), bullet->getTextures(), bullet->getMeshes(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		temp->_transform.scale(glm::vec3(1.0f), 0.01f);
		_bullets.push_back(temp);
	}
}

void Crosshair::addBullets(Bullet* bullet)
{
}

bool AR::shoot(glm::vec3& camera, glm::vec3& pos)
{
	if (!(_ammoCount >= _ammo) && getFire())
	{
		setDir(camera);
		_dirVec = glm::normalize(_dirVec);

		_bullets[_index]->_rigidBody.setVelocity(_dirVec * 25.0f);
		_bullets[_index]->_rigidBody._position = pos;

		_bullets[_index]->setActive(true);
		_index++;
		_ammoCount++;
		if (_index >= _bullets[_index]->getLife() / _firerate)
			_index = 0;
		Cappuccino::SoundSystem::playSound2D(soundHandle, groupHandle, Cappuccino::SoundSystem::ChannelType::SoundEffect);
		return true;
	}
	return false;
}

bool Pistol::shoot(glm::vec3& camera, glm::vec3& pos)
{
	if (getFire())
	{
		setDir(camera);
		_dirVec = glm::normalize(_dirVec);

		_bullets[_index]->_rigidBody.setVelocity(_dirVec * 25.0f);
		_bullets[_index]->_rigidBody._position = pos;

		_bullets[_index]->setActive(true);
		_index++;
		if (_index >= _bullets[_index]->getLife() / _firerate)
			_index = 0;
		Cappuccino::SoundSystem::playSound2D(soundHandle, groupHandle, Cappuccino::SoundSystem::ChannelType::SoundEffect);
		return true;
	}
	return false;
}

bool SG::shoot(glm::vec3& camera, glm::vec3& pos)
{
	if (getFire())
	{
		setDir(camera);
		_dirVec = glm::normalize(_dirVec);

		//Need to figure out spread
		for (unsigned i = 0; i < _pellets; i++)
		{
			_bullets[_index]->_rigidBody.setVelocity(_dirVec * 25.0f);
			_bullets[_index]->_rigidBody._position = pos;

			_bullets[_index]->setActive(true);
			_index++;
			if (_index >= _bullets[_index]->getLife() / _firerate)
				_index = 0;
		}
		
		Cappuccino::SoundSystem::playSound2D(soundHandle, groupHandle, Cappuccino::SoundSystem::ChannelType::SoundEffect);
		return true;
	}
	return false;
}

bool Crosshair::shoot(glm::vec3& camera, glm::vec3& pos)
{
	return false;
}