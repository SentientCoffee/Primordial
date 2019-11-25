#include "..\include\Enemy.h"
#include "Cappuccino/CappMath.h"
#include "Cappuccino/HitBoxLoader.h"
#include "Cappuccino/SoundSystem.h"
#include "glm/gtx/rotate_vector.hpp"
#include "Cappuccino/CappMacros.h"

#include "Cappuccino/Input.h"
#include "Cappuccino/Events.h"

#include "Class.h"

Enemy::Enemy(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs, const std::optional<float>& mass)
	:Cappuccino::GameObject(*SHADER, textures, meshs, mass), triggerVolume(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(20.0f, 20.0f, 20.0f))
{
	hp = 1.0f;
	auto& m = std::vector<Cappuccino::Mesh*>{ new Cappuccino::Mesh("NUTtest.obj") };
	auto& t = std::vector<Cappuccino::Texture*>{ new Cappuccino::Texture("metal.png",Cappuccino::TextureType::DiffuseMap) };
	for (unsigned i = 0; i < 18; i++)
		_deathParticles.push_back(new Particle(*SHADER, t, m));
}

void Enemy::childUpdate(float dt)
{
	_enemyGun->setDelay(dt);
}

bool Enemy::dead()
{

	if (hp <= 0.0f) {
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
	else
		return false;
}

void Enemy::attack(GameObject* other, float speed)
{

	static bool first = false;
	if (!_targetAquired) {
		first = false;
		_rigidBody.setVelocity(glm::vec3(0.0f));
		wander();
		return;
	}

	if (!first) {

		Cappuccino::SoundSystem::playSound2D(_sound, _group, Cappuccino::SoundSystem::ChannelType::SoundEffect);
		first = true;
	}

	auto newPos = (other->_rigidBody._position /*+ other->_rigidBody._vel/4.0f*/) - _rigidBody._position;

	auto normOther = glm::normalize(newPos);
	normOther.y -= 0.08f;//cause i dont like the bullets being in my face

	_rigidBody.setVelocity(normOther * 3.0f);

	_enemyGun->shoot(glm::vec3(normOther), _rigidBody._position);
}

void Enemy::wander()
{

	auto norm = glm::normalize(glm::vec3(sinf(glfwGetTime() * 2.0f), -cosf(glfwGetTime() * 2.0f), -1.0f));

	_rigidBody.setVelocity(-norm * 2.5f);
}

void Enemy::hurt(float damage)
{
	hp -= damage;
	Cappuccino::SoundSystem::playSound2D(_hurtSound, _group, Cappuccino::SoundSystem::ChannelType::SoundEffect);
}

void Enemy::setHurtSound(const std::string& path)
{
	_hurtSound = Cappuccino::SoundSystem::load2DSound(path);
	_group = Cappuccino::SoundSystem::createChannelGroup("hurt");
}


 Sentry::Sentry(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs, const std::optional<float>& mass) :
	Enemy(SHADER, textures, meshs, mass)
{
	auto loader = Cappuccino::HitBoxLoader("./Assets/Meshes/Hitboxes/SentryBox.obj");

	for (auto x : loader._boxes)
		_rigidBody._hitBoxes.push_back(x);


	_enemyGun = new AR(*SHADER, std::vector<Cappuccino::Texture*>{}, meshs, "testWeapon", 1.0f, 0.1f, 200);

	_enemyGun->setShootSound("SentryLaser.wav", "SentryGroup");

	_sound = Cappuccino::SoundSystem::load2DSound("targetAquired.wav");
	_hurtSound = Cappuccino::SoundSystem::load2DSound("machineHurt.wav");
	_group = Cappuccino::SoundSystem::createChannelGroup("robotGroup");
	hp = 50.0f;


	testMorph = new Cappuccino::Mesh("Sentry2.obj");
	testMorph->loadMesh();

	auto testMorph1 = new Cappuccino::Mesh("Sentry3.obj");
	testMorph1->loadMesh();

	animation = new Cappuccino::Animation(std::vector<Cappuccino::Mesh*>{_meshes.back(), testMorph, testMorph1, new Cappuccino::Mesh(*_meshes.back())});
}

Ghoul::Ghoul(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs, const std::optional<float>& mass) :
	Enemy(SHADER, textures, meshs, mass)
{
	auto loader = Cappuccino::HitBoxLoader("./Assets/Meshes/Hitboxes/SentryBox.obj");

	for (auto x : loader._boxes)
		_rigidBody._hitBoxes.push_back(x);

	_enemyGun = new AR(*SHADER, std::vector<Cappuccino::Texture*>{}, meshs, "testWeapon", 1.0f, 0.1f, 200);

	_enemyGun->setShootSound("SentryLaser.wav", "SentryGroup");

	_sound = Cappuccino::SoundSystem::load2DSound("targetAquired.wav");
	_hurtSound = Cappuccino::SoundSystem::load2DSound("machineHurt.wav");
	_group = Cappuccino::SoundSystem::createChannelGroup("robotGroup");

	hp = 70.0f;
	_jump = 3.0f;
	_jumpAnim = 1.0f;
}

void Ghoul::attack(GameObject* other, float speed)
{
	if (!_targetAquired) {
		_rigidBody.setVelocity(glm::vec3(0.0f));
		wander();
	}
	else
	{
		auto newPos = (other->_rigidBody._position /*+ other->_rigidBody._vel/4.0f*/) - _rigidBody._position;

		float dist = glm::length(newPos);

		auto normOther = glm::normalize(newPos);

		if (_jumpAnim == 1.0f)
		{
			_rigidBody.setVelocity(normOther * 3.0f);
			_jump -= speed;
		}
		else
			_jumpAnim -= speed;

		if (_jumpAnim <= 0.0f)
		{
			_jumpAnim = 1.0f;
		}

		if (_jump <= 0.0f)
		{
			_jumpAnim -= speed;
			if (dist >= 1.0f)
			{
				_rigidBody.setVelocity(_rigidBody._vel + other->_rigidBody._vel);
			}
			else
				_rigidBody.setVelocity(_rigidBody._vel * 3.0f);
			_jump = 2.0f;
		}

	}
}

void Ghoul::wander()
{

	auto norm = glm::normalize(glm::vec3(sinf(glfwGetTime() * 2.0f), 0.0f, -sinf(glfwGetTime() * 2.0f)));

	_rigidBody.setVelocity(-norm * 2.5f);
}

Sentinel::Sentinel(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::optional<float>& mass)
	:Enemy(SHADER,textures,meshes,mass)
{
	auto loader = Cappuccino::HitBoxLoader("./Assets/Meshes/Hitboxes/SentryBox.obj");

	for (auto x : loader._boxes)
		_rigidBody._hitBoxes.push_back(x);

	_enemyGun = new AR(*SHADER, std::vector<Cappuccino::Texture*>{}, std::vector<Cappuccino::Mesh*>{}, "Mega Big Machine Gun", 1.0f, 0.1f, INT_MAX);

	_enemyGun->setShootSound("bigCannon.wav", "SentryGroup");
	
	setHurtSound("machineHurt.wav");

	hp = 1000.0f;
}

void Sentinel::wander()
{
}

void Sentinel::attack(GameObject* other, float speed)
{

	if (5 + rand() % 10 >= 5 || !_targetAquired)
		return;
	
	auto newPos = (other->_rigidBody._position /*+ other->_rigidBody._vel/4.0f*/) - _rigidBody._position;
	
	auto normOther = glm::normalize(newPos);
	normOther.y -= 0.08f;//cause i dont like the bullets being in my face
	
	_enemyGun->shoot(glm::vec3(normOther), _rigidBody._position);
}
