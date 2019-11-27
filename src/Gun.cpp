#include "Gun.h"
#include "Class.h"
#include "Cappuccino/SoundSystem.h"
#include "glm/gtx/rotate_vector.hpp"

Gun::Gun(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string weapon, const float damage, const float firerate, const int ammo)
	:GameObject(SHADER, textures, meshes, 1.0f), _weapon(weapon), _damage(damage), _firerate(firerate), _ammo(ammo)
{

}

Gun::Gun(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	: GameObject(*SHADER, textures, meshes)
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

void Gun::childUpdate(float dt)
{
}

void Gun::setShootSound(const std::string& path, const std::string& groupName)
{
	soundHandle = Cappuccino::SoundSystem::load2DSound(path);
	groupHandle = Cappuccino::SoundSystem::createChannelGroup(groupName);
}


AR::AR(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string& weapon, const float damage, const float firerate, const int ammo)
	:Gun(SHADER, textures, meshes, weapon, damage, firerate, ammo)
{
	_offset = glm::vec3(0.0f, -0.05f, 0.05f);
}

Pistol::Pistol(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string& weapon, const float damage, const float firerate, const int ammo)
	: Gun(SHADER, textures, meshes, weapon, damage, firerate, ammo)
{
	_offset = glm::vec3(0.0f);

}

SG::SG(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string& weapon, const float damage, const float firerate, const int ammo, const int pellets)
	: Gun(SHADER, textures, meshes, weapon, damage, firerate, ammo), _pellets(pellets)
{
	_offset = glm::vec3(0.0f);
}

void Gun::addBullets(Bullet* bullet)
{
	int indexMax = (bullet->getLife() / _firerate) + 1.0f;
	for (unsigned i = 0; i < indexMax; i++)
	{
		for (auto x : bullet->getMeshes())
			x->loaded = true;
		for (auto x : bullet->getTextures())
			x->setLoaded(true);
		Bullet* temp = new Bullet(bullet->getShader(), bullet->getTextures(), bullet->getMeshes(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		temp->_transform._scaleMat = bullet->_transform._scaleMat;
		temp->_rigidBody._hitBoxes.push_back(Cappuccino::HitBox(temp->_rigidBody._position, glm::vec3(temp->_transform._scaleMat * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))));
		temp->_rigidBody._hitBoxes.push_back(Cappuccino::HitBox(temp->_rigidBody._position, glm::vec3(temp->_transform._scaleMat * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))));
		_bullets.push_back(temp);
	}
}

void SG::addBullets(Bullet* bullet)
{
	int indexMax = (bullet->getLife() / _firerate + 1.0f) * _pellets;
	for (unsigned i = 0; i < indexMax; i++)
	{
		for (auto x : bullet->getMeshes())
			x->loaded = true;
		for (auto x : bullet->getTextures())
			x->setLoaded(true);
		Bullet* temp = new Bullet(bullet->getShader(), bullet->getTextures(), bullet->getMeshes(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		temp->_transform.scale(glm::vec3(1.0f), 0.1f);
		temp->_rigidBody._hitBoxes.push_back(Cappuccino::HitBox(temp->_rigidBody._position, glm::vec3(temp->_transform._scaleMat * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))));
		temp->_rigidBody._hitBoxes.push_back(Cappuccino::HitBox(temp->_rigidBody._position, glm::vec3(temp->_transform._scaleMat * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))));
		_bullets.push_back(temp);
	}
}

bool Gun::shoot(glm::vec3& camera, glm::vec3& pos)
{
	if (!(_ammoCount >= _ammo) && getFire())
	{
		setDir(camera);
		_dirVec = glm::normalize(_dirVec);

		_bullets[_index]->_rigidBody.setVelocity(_dirVec * 75.0f);
		_bullets[_index]->_rigidBody._position = pos;

		_bullets[_index]->setActive(true);
		_index++;
		_ammoCount++;
		if (_index >= _bullets[_index - 1]->getLife() / _firerate)
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

		_bullets[_index]->_rigidBody.setVelocity(_dirVec * 75.0f);
		_bullets[_index]->_rigidBody._position = pos;

		_bullets[_index]->setActive(true);
		_index++;
		if (_index >= _bullets[_index - 1]->getLife() / _firerate)
			_index = 0;
		Cappuccino::SoundSystem::playSound2D(soundHandle, groupHandle, Cappuccino::SoundSystem::ChannelType::SoundEffect);
		return true;
	}
	return false;
}

bool SG::shoot(glm::vec3& camera, glm::vec3& pos)
{
	if (!(_ammoCount >= _ammo) && getFire())
	{
		setDir(camera);
		_dirVec = glm::normalize(_dirVec);

		//Need to figure out spread
		for (unsigned i = 0; i < _pellets; i++)
		{

			_bullets[_index]->_rigidBody._position = pos;

			auto sign = rand() % 2 == 0 ? 1.0f : -1.0f;
			_dirVec.x += ((float)(rand() % 3) / 100.0f)*(rand() % 2 == 0 ? sign : 1.0f);
			_dirVec.y += ((float)(rand() % 3) / 100.0f)*(rand() % 2 == 0 ? sign : 1.0f);
			_dirVec.z += ((float)(rand() % 3) / 100.0f)*(rand() % 2 == 0 ? sign : 1.0f);
			_bullets[_index]->_rigidBody.setVelocity((75.0f * _dirVec * ((float)(1 + rand() % 4))));


			_bullets[_index]->setActive(true);
			_index++;
			if (_index >= _bullets[_index - 1]->getLife() / _firerate * _pellets)
				_index = 0;
		}

		_ammoCount++;
		Cappuccino::SoundSystem::playSound2D(soundHandle, groupHandle, Cappuccino::SoundSystem::ChannelType::SoundEffect);
		return true;
	}
	return false;
}

GL::GL(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string& weapon, const float damage, const float firerate, const int ammo)
	:Gun(SHADER, textures, meshes, weapon, damage, firerate, ammo), _aoe(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f))
{
}

bool GL::shoot(glm::vec3& camera, glm::vec3& pos)
{
	if (!(_ammoCount >= _ammo) && getFire())
	{
		setDir(camera);
		_dirVec = glm::normalize(_dirVec);
		_dirVec.y += 0.1f;

		_bullets[_index]->_rigidBody.setVelocity(_dirVec * 50.0f);

		if (_bullets[_index]->_rigidBody._accel.y != 0.0f)
			_bullets[_index]->_rigidBody._accel.y = 0.0f;

		_bullets[_index]->_rigidBody._position = pos;
		_bullets[_index]->_rigidBody.setGrav(true);

		_bullets[_index]->setActive(true);
		_index++;
		_ammoCount++;
		if (_index >= _bullets[_index - 1]->getLife() / _firerate)
			_index = 0;
		Cappuccino::SoundSystem::playSound2D(soundHandle, groupHandle, Cappuccino::SoundSystem::ChannelType::SoundEffect);
		return true;
	}
	return false;
}

void GL::addBullets(Bullet* bullet)
{
	int indexMax = (bullet->getLife() / _firerate) + 1.0f;
	for (unsigned i = 0; i < indexMax; i++)
	{
		for (auto x : bullet->getMeshes())
			x->loaded = true;
		for (auto x : bullet->getTextures())
			x->setLoaded(true);
		Bullet* temp = new Bullet(bullet->getShader(), bullet->getTextures(), bullet->getMeshes(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		temp->_transform._scaleMat = bullet->_transform._scaleMat;
		temp->_rigidBody._hitBoxes.push_back(Cappuccino::HitBox(temp->_rigidBody._position, glm::vec3(temp->_transform._scaleMat * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))));
		temp->_rigidBody._hitBoxes.push_back(Cappuccino::HitBox(temp->_rigidBody._position, glm::vec3(temp->_transform._scaleMat * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))));
		_bullets.push_back(temp);
	}
}
