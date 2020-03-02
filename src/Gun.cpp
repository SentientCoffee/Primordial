#include "Gun.h"
#include "Class.h"
#include "Cappuccino/SoundSystem.h"
#include "glm/gtx/rotate_vector.hpp"
#include "Enemy.h"

Gun::Gun(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string weapon, const float damage, const float firerate, const int ammo, bool isEnemy, float yBulletOffset)
	: GameObject(SHADER, textures, meshes, 1.0f), _weapon(weapon), _damage(damage), _firerate(firerate), _ammo(ammo), _isEnemy(isEnemy), _yBulletOffset(yBulletOffset)
{
	id = "UI";
}

Gun::Gun(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	: GameObject(*SHADER, textures, meshes)
{
	id = "UI";
}

void Gun::setDelay(float dt)
{
	if (_delay) {
		//std::cout << _delay << std::endl;
		_delay += dt;
		if (_delay >= _firerate)
			_delay = 0.0f;
	}
}

bool Gun::getFire()
{
	if (!_delay) {
		return true;
	}
	else
		return false;
}

void Gun::childUpdate(float dt)
{
	//if (_delay) {
	//	_delay += dt;
	//	if (_delay >= _firerate)
	//		_delay = 0.0f;
	//}
		
}

void Gun::setShootSound(const std::string& path, const std::string& groupName)
{
	soundHandle = Cappuccino::SoundSystem::load2DSound(path);
	groupHandle = Cappuccino::SoundSystem::createChannelGroup(groupName);
}

void Gun::setYBulletOffset(float offset)
{
	_yBulletOffset = offset;
}


AR::AR(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string& weapon, const float damage, const float firerate, const int ammo, bool isEnemy)
	: Gun(SHADER, textures, meshes, weapon, damage, firerate, ammo, isEnemy)
{
	//_offset = glm::vec3(0.0f, -0.05f, 0.05f);
}


Pistol::Pistol(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string& weapon, const float damage, const float firerate, const int ammo, bool isEnemy)
	: Gun(SHADER, textures, meshes, weapon, damage, firerate, ammo, isEnemy)
{
	_offset = glm::vec3(0.0f);

}

SG::SG(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string& weapon, const float damage, const float firerate, const int ammo, const int pellets, bool isEnemy)
	: Gun(SHADER, textures, meshes, weapon, damage, firerate, ammo, isEnemy), _pellets(pellets)
{
	_offset = glm::vec3(0.0f);
}

void Gun::addBullets(Bullet* bullet)
{
	int indexMax = (bullet->getLife() / _firerate) + 1.0f;
	if (_isEnemy) {
		for (unsigned i = 0; i < 15; i++)
		{
			for (auto& x : bullet->getMeshes())
				x->loaded = true;
			for (auto& x : bullet->getTextures())
				x->setLoaded(true);
			Bullet* temp = new Bullet(bullet->getShader(), bullet->getTextures(), bullet->getMeshes(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			temp->_transform._scaleMat = bullet->_transform._scaleMat;
			temp->_rigidBody._hitBoxes.push_back(Cappuccino::HitBox(temp->_rigidBody._position, glm::vec3(temp->_transform._scaleMat * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))));
			temp->_rigidBody._hitBoxes.push_back(Cappuccino::HitBox(temp->_rigidBody._position, glm::vec3(temp->_transform._scaleMat * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))));
			_bullets.push_back(temp);
		}
	}
	else
		for (unsigned i = 0; i < indexMax; i++)
		{
			for (auto& x : bullet->getMeshes())
				x->loaded = true;
			for (auto& x : bullet->getTextures())
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
		for (auto& x : bullet->getMeshes())
			x->loaded = true;
		for (auto& x : bullet->getTextures())
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
	if ((!(_ammoCount >= _ammo) || _isEnemy) && getFire())
	{
		_delay += 0.01;
		setDir(camera);
		_dirVec = glm::normalize(_dirVec);

		_bullets[_index]->_rigidBody.setVelocity(_dirVec * 75.0f);
		_bullets[_index]->_rigidBody._position = pos;
		_bullets[_index]->_rigidBody._position.y += _yBulletOffset;
		_bullets[_index]->_rigidBody._hitWall = false;

		_bullets[_index]->setActive(true);
		_index++;
		_ammoCount++;
		if (_index >= _bullets[_index - 1]->getLife() / _firerate || _isEnemy && _index > _bullets.size() - 1)
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
		_delay += 0.01;
		setDir(camera);
		_dirVec = glm::normalize(_dirVec);

		_bullets[_index]->_rigidBody.setVelocity(_dirVec * 75.0f);
		_bullets[_index]->_rigidBody._position = pos;
		_bullets[_index]->_rigidBody._hitWall = false;

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
		_delay += 0.01;
		setDir(camera);
		_dirVec = glm::normalize(_dirVec);

		//Need to figure out spread
		for (unsigned i = 0; i < _pellets; i++)
		{

			_bullets[_index]->_rigidBody._position = pos;
			_bullets[_index]->_rigidBody._hitWall = false;

			auto sign = Cappuccino::randomInt(0.0,1.0)  == 0 ? 1.0f : -1.0f;
			_dirVec.x += ((Cappuccino::randomFloat(0.0, 2.0)) / 100.0f) * (Cappuccino::randomInt(0.0, 1.0) == 0 ? sign : 1.0f);
			_dirVec.y += ((Cappuccino::randomFloat(0.0, 2.0)) / 100.0f) * (Cappuccino::randomInt(0.0, 1.0) == 0 ? sign : 1.0f);
			_dirVec.z += ((Cappuccino::randomFloat(0.0, 2.0)) / 100.0f) * (Cappuccino::randomInt(0.0, 1.0) == 0 ? sign : 1.0f);
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

GL::GL(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string& weapon, const float damage, const float firerate, const int ammo, bool isEnemy)
	:Gun(SHADER, textures, meshes, weapon, damage, firerate, ammo, isEnemy), _aoe(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f))
{
}

bool GL::shoot(glm::vec3& camera, glm::vec3& pos)
{
	if (!(_ammoCount >= _ammo) && getFire())
	{
		_delay += 0.01;
		setDir(camera);
		_dirVec = glm::normalize(_dirVec);
		_dirVec.y += 0.1f;

		_bullets[_index]->_rigidBody.setVelocity(_dirVec * 50.0f);

		if (_bullets[_index]->_rigidBody._accel.y != 0.0f)
			_bullets[_index]->_rigidBody._accel.y = 0.0f;

		_bullets[_index]->_rigidBody._position = pos;
		_bullets[_index]->_rigidBody._hitWall = false;
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
		//	temp->_rigidBody._moveable = true;
		_bullets.push_back(temp);
	}
}

void GL::specialCollisionBehaviour(const std::vector<Enemy*>& enemies)
{
	for (auto enemies : enemies) {
		for (auto bullets : _bullets) {
			auto newPos = enemies->_rigidBody._position - bullets->_rigidBody._position;
			auto dist = glm::length(newPos);
			if (dist <= 10.0f)
				enemies->hurt(this->_damage / dist);
		}
	}
	_explosionSound.play();
}

Melee::Melee(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string& weapon, const float damage, const float firerate, bool isEnemy)
	:Gun(SHADER, textures, meshes, weapon, damage, firerate, (unsigned)-1, isEnemy)
{
	_offset = glm::vec3(0.0f);
}

bool Melee::shoot(glm::vec3& camera, glm::vec3& pos)
{
	return false;
}

void Melee::addBullets(Bullet* bullet)
{

}

HSAR::HSAR(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string& weapon, const float damage, const float firerate, const int ammo, bool isEnemy)
	:Gun(SHADER, textures, meshes, weapon, damage, firerate, ammo, isEnemy)
{
	_hitScanDamage = damage;
	_offset = glm::vec3(0.0f, -0.05f, 0.05f);
	_isHitscan = true;
	_damage = 0.0f;
}

bool HSAR::shoot(glm::vec3& camera, glm::vec3& pos)
{
	if (!(_ammoCount >= _ammo) && getFire())
	{
		_delay += 0.01;
		setDir(camera);
		_dirVec = glm::normalize(_dirVec);

		_hitscanRay = Cappuccino::Ray(camera, pos);
		std::vector<std::string> ids;
		ids.push_back("Player");
		ids.push_back("Bullet");

		Cappuccino::GameObject* hitObject = this->getFirstIntersect(_hitscanRay,ids,true);//first object hit
		for (auto y : this->gameObjects)//for all gameobjects
			if (y->id == "Enemy") {//if the object is an enemy
				if (y->isActive() && y == hitObject) {
					static_cast<Enemy*>(y)->hurt(_hitScanDamage);//toggle the hud
				}
			}
		_ammoCount++;
		Cappuccino::SoundSystem::playSound2D(soundHandle, groupHandle, Cappuccino::SoundSystem::ChannelType::SoundEffect);
		//_damage = _tempDamage;
		return true;
	}
	_hitscanRay = Cappuccino::Ray(glm::vec3(0.0f), glm::vec3(0.0f));

	return false;
}

void HSAR::addBullets(Bullet* bullet)
{
}

void HSAR::specialCollisionBehaviour(const std::vector<Enemy*>& enemies)
{
	//_damage = 0.0f;
}

