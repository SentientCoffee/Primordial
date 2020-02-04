#include "..\include\Enemy.h"
#include "Cappuccino/CappMath.h"
#include "Cappuccino/HitBoxLoader.h"
#include "Cappuccino/SoundSystem.h"
#include "glm/gtx/rotate_vector.hpp"
#include "Cappuccino/CappMacros.h"

#include "Cappuccino/Input.h"
#include "Cappuccino/Events.h"

#include "Class.h"
#include <Cappuccino/ResourceManager.h>

Enemy::Enemy(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs, const std::optional<float>& mass)
	: GameObject(*SHADER, textures, meshs, mass), triggerVolume(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(20.0f, 20.0f, 20.0f))
{
	_hp = 1.0f;
	_rigidBody._moveable = true;
	_rigidBody.setGrav(true);
	_rigidBody._canTouch = true;
	_rigidBody._creature = true;
	this->id = "Enemy";
}

void Enemy::childUpdate(float dt)
{
	_enemyGun->setDelay(dt);
}

bool Enemy::dead()
{

	if (_hp <= 0.0f) {
		for (unsigned i = 0; i < _deathParticles.size(); i++) {
			_deathParticles[i]->setActive(true);
			_deathParticles[i]->_rigidBody.setGrav(false);
			_deathParticles[i]->_rigidBody._position = _rigidBody._position;
			_deathParticles[i]->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.1f);
			_deathParticles[i]->_transform.rotate(glm::vec3(1.0f, 0.0f, 1.0f), i);
			_deathParticles[i]->_rigidBody.setVelocity(glm::vec3(cosf(i) * 2.0f, sinf(i) * 2.0f, 0.0f) * 5.0f);
			_deathParticles[i]->_rigidBody._vel *= 2.0f;
		}
		setActive(false);
		return true;
	}

	return false;
}

void Enemy::attack(Class* other, float dt)
{

	if (!_targetAquired) {
		wander(dt);
	}
	else
	{
		if (!_encountered) {

			Cappuccino::SoundSystem::playSound2D(_sound, _group, Cappuccino::SoundSystem::ChannelType::SoundEffect);
			_encountered = true;
		}

		auto newPos = (other->_rigidBody._position) - _rigidBody._position;

		float dist = glm::length(newPos);

		auto normOther = glm::normalize(newPos);
		normOther.y -= 0.08f;

		if (dist >= _distance)
			_rigidBody.setVelocity(normOther * 3.0f);

		_enemyGun->shoot(glm::vec3(normOther), _rigidBody._position);
	}
}

void Enemy::wander(float dt)
{

	_wanderCycle -= dt;
	auto norm = normalize(glm::vec3(1.0f, 0.0f, 0.0f));

	if (_wanderCycle <= -10.0f)
		_wanderCycle = 10.0f;
	else if (_wanderCycle <= 0.0f)
		_rigidBody.setVelocity(-norm * 2.5f);
	else
		_rigidBody.setVelocity(norm * 2.5f);
}

void Enemy::hurt(float damage)
{
	if (_shield > 0) {
		_shield -= damage;
		if (_shield < 0)
		{
			_hp -= _shield;
			_shield = 0;
		}
	}
	else {
		_hp -= damage;
	}
	Cappuccino::SoundSystem::playSound2D(_hurtSound, _group, Cappuccino::SoundSystem::ChannelType::SoundEffect);
}

void Enemy::setHurtSound(const std::string& path)
{
	_hurtSound = Cappuccino::SoundSystem::load2DSound(path);
	_group = Cappuccino::SoundSystem::createChannelGroup("hurt");
}

RoboGunner::RoboGunner(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs) :
	Enemy(SHADER, textures, meshs)
{
	auto loader = Cappuccino::HitBoxLoader("./Assets/Meshes/Hitboxes/BotBox.obj");

	for (auto x : loader._boxes)
		_rigidBody._hitBoxes.push_back(x);


	_enemyGun = new AR(*SHADER, std::vector<Cappuccino::Texture*>{}, meshs, "testWeapon", 1.0f, 0.1f, 200);

	_enemyGun->setShootSound("SentryLaser.wav", "SentryGroup");

	_sound = Cappuccino::SoundSystem::load2DSound("targetAquired.wav");
	_hurtSound = Cappuccino::SoundSystem::load2DSound("machineHurt.wav");
	_group = Cappuccino::SoundSystem::createChannelGroup("robotGroup");
	_maxHp = 200.0f;
	_hp = _maxHp;
	_maxShield = 200.0f;
	_shield = _maxShield;
	_distance = 10.0f;
	_weight = 3.0f;
}

Grunt::Grunt(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs) :
	Enemy(SHADER, textures, meshs)
{
	auto loader = Cappuccino::HitBoxLoader("./Assets/Meshes/Hitboxes/BotBox.obj");

	for (auto x : loader._boxes)
		_rigidBody._hitBoxes.push_back(x);


	_enemyGun = new AR(*SHADER, std::vector<Cappuccino::Texture*>{}, meshs, "testWeapon", 1.0f, 0.1f, 200);

	_enemyGun->setShootSound("SentryLaser.wav", "SentryGroup");

	_sound = Cappuccino::SoundSystem::load2DSound("targetAquired.wav");
	_hurtSound = Cappuccino::SoundSystem::load2DSound("machineHurt.wav");
	_group = Cappuccino::SoundSystem::createChannelGroup("robotGroup");
	_maxHp = 75.0f;
	_hp = _maxHp;
	_maxShield = 50.0f;
	_shield = _maxShield;
	_distance = 10.0f;
	_weight = 1.0f;
}

Captain::Captain(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs) :
	Enemy(SHADER, textures, meshs)
{
	auto loader = Cappuccino::HitBoxLoader("./Assets/Meshes/Hitboxes/BotBox.obj");

	//std::swap(loader._boxes[0], loader._boxes[1]);
	for (auto x : loader._boxes)
		_rigidBody._hitBoxes.push_back(x);


	_enemyGun = new AR(*SHADER, std::vector<Cappuccino::Texture*>{}, meshs, "testWeapon", 1.0f, 0.1f, 200);

	_enemyGun->setShootSound("SentryLaser.wav", "SentryGroup");

	_sound = Cappuccino::SoundSystem::load2DSound("targetAquired.wav");
	_hurtSound = Cappuccino::SoundSystem::load2DSound("machineHurt.wav");
	_group = Cappuccino::SoundSystem::createChannelGroup("robotGroup");
	_maxHp = 100.0f;
	_hp = _maxHp;
	_maxShield = 100.0f;
	_shield = _maxShield;
	_distance = 15.0f;
	_weight = 2.0f;
}

Sentry::Sentry(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::optional<float>& mass) :
	Enemy(SHADER, textures, meshes, mass)
{
	auto loader = Cappuccino::HitBoxLoader("./Assets/Meshes/Hitboxes/SentryBox.obj");

	for (auto x : loader._boxes)
		_rigidBody._hitBoxes.push_back(x);


	_enemyGun = new AR(*SHADER, {}, meshes, "testWeapon", 1.0f, 0.1f, 200);

	_enemyGun->setShootSound("SentryLaser.wav", "SentryGroup");

	_sound = Cappuccino::SoundSystem::load2DSound("targetAquired.wav");
	_hurtSound = Cappuccino::SoundSystem::load2DSound("machineHurt.wav");
	_group = Cappuccino::SoundSystem::createChannelGroup("robotGroup");
	_maxHp = 50.0f;
	_hp = _maxHp;
	_maxShield = 100.0f;
	_shield = _maxShield;
	_distance = 5.0f;
	_weight = 1.5f;

	triggerVolume = Cappuccino::HitBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(50.0f, 50.0f, 50.0f));

	testMorph = Cappuccino::MeshLibrary::loadMesh("Sentry 2", "Sentry2.obj");
	testMorph->loadMesh();

	auto testMorph1 = Cappuccino::MeshLibrary::loadMesh("Sentry 3", "Sentry3.obj");
	testMorph1->loadMesh();

	_animator.addAnimation(new Cappuccino::Animation(
		std::vector<Cappuccino::Mesh*>{ _meshes.back(), testMorph, testMorph1, new Cappuccino::Mesh(*_meshes.back()) },
		AnimationType::Idle));
	_animator.setLoop(AnimationType::Idle, true);

}

void Sentry::attack(Class* other, float dt)
{
	static bool first = false;
	if (!_targetAquired) {
		first = false;
		_rigidBody.setVelocity(glm::vec3(0.0f));
		wander(dt);
		return;
	}

	if (!first) {

		Cappuccino::SoundSystem::playSound2D(_sound, _group, Cappuccino::SoundSystem::ChannelType::SoundEffect);
		first = true;
	}

	auto newPos = other->_rigidBody._position - _rigidBody._position;

	float dist = glm::length(newPos);

	auto normOther = glm::normalize(newPos);
	auto perp = glm::normalize(cross(other->_rigidBody._position, normOther));
	//auto dottest = glm::dot(normOther, perp); //resulted in 0 so it is perpendicular

	//Uniform Catmull Rom Spline (Closed Loop)
	// Logically, I get the catmull rom position 0.2 along the curve, subtract position of sentry to get vector, 
	//normalize to get direction, then apply dir to sentry's velocity
	glm::vec3 crmPos = CatmullRom(dt,
		other->_rigidBody._position - (5.0f * normOther),
		other->_rigidBody._position - (5.0f * perp),
		other->_rigidBody._position + (5.0f * normOther),
		other->_rigidBody._position + (5.0f * perp));

	glm::vec3 dir = glm::normalize(crmPos - _rigidBody._position);

	_rigidBody.setVelocity(dir * 5.0f);
	//_rigidBody._position =crmPos;

	_enemyGun->shoot(glm::vec3(normOther), _rigidBody._position);
}

glm::vec3 Enemy::CatmullRom(float t, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	glm::mat4x4 catmull = { {-1.0f, 3.0f, -3.0f, 1.0f},
							{2.0f, -5.0f, 4.0f, -1.0f},
							{-1.0f, 0.0f, 1.0f, 0.0f},
							{0.0f, 2.0f, 0.0f, 0.0f} };

	glm::vec4 curve = { 1.0f, t, t * t, t * t * t };

	glm::mat4x3 waypoints = { p0, p1, p2, p3 };

	auto test1 = (0.5f * ((2.0f * p1) +
		((-p0 + p2) * t) +
		((2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * (t * t)) +
		((-p0 + 3.0f * p1 - 3.0f * p2 + p3) * (t * t * t)))); //reverse order of the catmull matrix but seems to circle the player extremely closely regardless of p0->p3 position


	auto test2 = 0.5f * (curve * catmull * waypoints); //gets stuck 


	auto test3 = 0.5f * (waypoints * catmull * curve); //circles a spot near the light?


	return test3;
}

void Sentry::wander(float dt)
{

	auto norm = glm::normalize(glm::vec3(sinf(glfwGetTime() * 2.0f), -cosf(glfwGetTime() * 2.0f), -1.0f));

	_rigidBody.setVelocity(-norm * 2.5f);
}


Ghoul::Ghoul(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs, const std::optional<float>& mass) :
	Enemy(SHADER, textures, meshs, mass)
{
	auto loader = Cappuccino::HitBoxLoader("./Assets/Meshes/Hitboxes/GhoulBox.obj");

	for (auto x : loader._boxes)
		_rigidBody._hitBoxes.push_back(x);

	_enemyGun = new AR(*SHADER, {}, meshs, "testWeapon", 1.0f, 0.1f, 200);

	_enemyGun->setShootSound("SentryLaser.wav", "SentryGroup");

	_sound = Cappuccino::SoundSystem::load2DSound("ghoulAgro.wav");
	_jumpSound = Cappuccino::SoundSystem::load2DSound("ghoulAgro2.wav");
	_hurtSound = Cappuccino::SoundSystem::load2DSound("ghoulAgro3.wav");
	_group = Cappuccino::SoundSystem::createChannelGroup("ghoulGroup");

	_maxHp = 70.0f;
	_hp = _maxHp;
	_maxShield = 0.0f;
	_shield = _maxShield;
	_jump = 3.0f;
	_jumpAnim = 1.0f;
	_distance = 1.0f;
	_weight = 0.5f;

	triggerVolume._size *= 2.0f;

}

void Ghoul::attack(Class* other, float dt)
{
	static bool first = false;
	if (!_targetAquired) {
		first = false;
		_rigidBody.setVelocity(glm::vec3(0.0f));
		wander(dt);
	}
	else
	{
		if (!first) {
			Cappuccino::SoundSystem::playSound2D(_sound, _group, Cappuccino::SoundSystem::ChannelType::SoundEffect);
			first = true;
		}

		auto newPos = (other->_rigidBody._position /*+ other->_rigidBody._vel/4.0f*/) - _rigidBody._position;

		float dist = glm::length(newPos);

		auto normOther = glm::normalize(newPos);

		normOther.y = 0.0f;
		static bool alreadyHit = false;
		if (_jumpAnim == 1.0f)
		{
			_rigidBody.setVelocity(normOther * 3.0f);
			_jump -= dt;
			alreadyHit = false;
		}
		else {
			_jumpAnim -= dt;
			float attackDist = 5.f;
			if (dist <= attackDist && !alreadyHit) {
				other->takeDamage(50.0f);
				alreadyHit = true;
			}
		}

		if (_jumpAnim <= 0.0f)
		{
			_jumpAnim = 1.0f;
		}

		if (_jump <= 0.0f)
		{
			_jumpAnim -= dt;

			if (dist >= _distance)
			{
				auto norm = glm::normalize(_rigidBody._position - other->_rigidBody._position);

				norm *= -1.0f;

				auto angle = (glm::dot(norm, other->_rigidBody._position + other->_rigidBody._vel) / (glm::length(norm) * glm::length(other->_rigidBody._position + other->_rigidBody._vel)));
				angle = glm::acos(angle);

				angle /= 100.0f;

				norm = glm::rotate(norm, -angle, glm::vec3(0.0f, 1.0f, 0.0f));

				norm.y = 0.0f;

				_rigidBody.setVelocity(norm * 20.0f);



			}
			else
				_rigidBody.setVelocity(_rigidBody._vel * 3.0f);

			_jump = 2.0f;
			Cappuccino::SoundSystem::playSound2D(_jumpSound, _group, Cappuccino::SoundSystem::ChannelType::SoundEffect);
		}

	}
}

void Ghoul::wander(float dt)
{

	auto norm = glm::normalize(glm::vec3(sinf(glfwGetTime() * 2.0f), 0.0f, -sinf(glfwGetTime() * 2.0f)));

	_rigidBody.setVelocity(-norm * 2.5f);
}

Squelch::Squelch(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs) :
	Enemy(SHADER, textures, meshs)
{
	auto loader = Cappuccino::HitBoxLoader("./Assets/Meshes/Hitboxes/GhoulBox.obj");

	for (auto x : loader._boxes)
		_rigidBody._hitBoxes.push_back(x);

	_enemyGun = new AR(*SHADER, std::vector<Cappuccino::Texture*>{}, meshs, "testWeapon", 1.0f, 0.1f, 200);

	_enemyGun->setShootSound("SentryLaser.wav", "SentryGroup");

	_sound = Cappuccino::SoundSystem::load2DSound("ghoulAgro3.wav");
	_hurtSound = Cappuccino::SoundSystem::load2DSound("ghoulAgro4.wav");
	_group = Cappuccino::SoundSystem::createChannelGroup("robotGroup");

	_maxHp = 50.0f;
	_hp = _maxHp;
	_maxShield = 0.0f;
	_shield = _maxShield;
	_distance = 2.0f;
	_weight = 1.0f;
}

void Squelch::attack(Class* other, float dt)
{
	if (!_targetAquired) {
		_rigidBody.setVelocity(glm::vec3(0.0f));
		wander(dt);
	}
	else
	{
		static bool entered = false;

		//play a sound at entry
		if (!entered) {
			Cappuccino::SoundSystem::playSound2D(_sound, _group, Cappuccino::SoundSystem::ChannelType::SoundEffect);
			entered = true;
		}

		auto newPos = (other->_rigidBody._position /*+ other->_rigidBody._vel/4.0f*/) - _rigidBody._position;

		float dist = glm::length(newPos);

		auto normOther = glm::normalize(newPos);
		normOther.y = 0.0f;

		static float bloat = 0.0f;
		static glm::mat4 originalScaleMat(1.0f);
		static bool firstPrime = true;


		if (_primed) {
			if (firstPrime) {
				originalScaleMat = _transform._scaleMat;
				firstPrime = false;
			}
			_timer -= dt;
			_rigidBody.setVelocity(glm::vec3(0.0f));

			bloat += dt;
			bloat /= 10.0f;

			_transform._scaleMat[0].x += 4 * bloat;
			_transform._scaleMat[2].z += 4 * bloat;
		}

		if (_timer <= 0.0f)
		{
			bloat = 0.0f;
			firstPrime = true;
			entered = false;
			_primed = false;
			_hp = 0.0f;
			_timer = 1.0f;
			_transform._scaleMat = originalScaleMat;

			if (dist <= 5.0f)
				other->takeDamage(/*2.5f / dist * 110.0f*/1000.f);

			return;
		}

		if (dist >= _distance && !_primed)
			_rigidBody.setVelocity(normOther * 12.5f);
		else
			_primed = true;
	}
}

void Squelch::wander(float dt)
{
	//maybe some sort of hiding/standing animation
}

Sentinel::Sentinel(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::optional<float>& mass)
	:Enemy(SHADER, textures, meshes, mass)
{
	auto loader = Cappuccino::HitBoxLoader("./Assets/Meshes/Hitboxes/SentryBox.obj");

	for (auto x : loader._boxes)
		_rigidBody._hitBoxes.push_back(x);

	_enemyGun = new AR(*SHADER, {}, {}, "Mega Big Machine Gun", 1.0f, 0.1f, INT_MAX);

	_enemyGun->setShootSound("bigCannon.wav", "SentryGroup");

	setHurtSound("machineHurt.wav");

	_hp = 1000.0f;
}

void Sentinel::wander(float dt)
{
}

void Sentinel::attack(Class* other, float dt)
{

	if (5 + rand() % 10 >= 5 || !_targetAquired)
		return;

	auto newPos = (other->_rigidBody._position /*+ other->_rigidBody._vel/4.0f*/) - _rigidBody._position;

	auto normOther = glm::normalize(newPos);
	normOther.y -= 0.08f;//cause i dont like the bullets being in my face

	_enemyGun->shoot(glm::vec3(normOther), _rigidBody._position);
}

Primordial::Primordial(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::optional<float>& mass)
	:Enemy(SHADER, textures, meshes, mass)
{
	auto loader = Cappuccino::HitBoxLoader("./Assets/Meshes/Hitboxes/SentryBox.obj");

	for (auto x : loader._boxes)
		_rigidBody._hitBoxes.push_back(x);

	_enemyGun = new AR(*SHADER, {}, {}, "Mega Big Machine Gun", 1.0f, 0.1f, INT_MAX);

	_enemyGun->setShootSound("bigCannon.wav", "SentryGroup");

	setHurtSound("machineHurt.wav");
}

void Primordial::wander(float dt)
{
}

void Primordial::attack(Class* other, float speed)
{
	if (_phases == 0)
	{
		_phases++;
		//spawn(3);
	}
	else if (_hp + _shield <= 0.8f * (_maxHp + _maxShield) && _phases == 1)
	{
		_phases++;
		//spawn(5);
	}
	else if (_hp + _shield <= 0.6f * (_maxHp + _maxShield) && _phases == 2)
	{
		_phases++;
		//spawn(7);
	}
	else if (_hp + _shield <= 0.4f * (_maxHp + _maxShield) && _phases == 3)
	{
		_phases++;
		//spawn(9);
	}
	else if (_hp + _shield <= 0.2f * (_maxHp + _maxShield) && _phases == 4)
	{
		_phases++;
		//spawn(11);
	}
}

Dino::Dino(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::optional<float>& mass)
	:Enemy(SHADER, textures, meshes, mass)
{
	auto loader = Cappuccino::HitBoxLoader("./Assets/Meshes/Hitboxes/SentryBox.obj");

	for (auto x : loader._boxes)
		_rigidBody._hitBoxes.push_back(x);

	_enemyGun = new AR(*SHADER, {}, {}, "Mega Big Machine Gun", 1.0f, 0.1f, INT_MAX);

	_enemyGun->setShootSound("bigCannon.wav", "SentryGroup");

	setHurtSound("machineHurt.wav");

}

void Dino::wander(float dt)
{
}

void Dino::attack(Class* other, float speed)
{
}
