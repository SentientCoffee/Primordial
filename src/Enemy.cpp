#include "..\include\Enemy.h"
#include "Cappuccino/CappMath.h"
#include "Cappuccino/HitBoxLoader.h"
#include "Cappuccino/SoundSystem.h"
#include "glm/gtx/rotate_vector.hpp"
#include "Cappuccino/CappMacros.h"
#include "Cappuccino/Application.h"

#include "Cappuccino/Input.h"
#include "Cappuccino/Events.h"
#include "Cappuccino\Random.h"

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
	_hud->toggleHud();
	_camera.getRight() = glm::normalize(glm::cross(_camera.getFront(), _camera.getUp()));

}

void Enemy::childUpdate(float dt)
{
	_camera.setPosition(_rigidBody._position);
	if (_shieldTimer > 0.0f) {
		_shieldTimer -= dt;
	}
	else if (_shieldTimer <= 0.0f) {
		if (_shield < _maxShield)
			_shield += 0.25f;
	}

	_enemyGun->setDelay(dt);
	_hud->setHealth(_hp);
	_hud->setShield(_shield);
	_hud->updateHud(dt);

}

bool Enemy::dead()
{

	if (_hp <= 0.0f) {
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
//the scale tho
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
		Cappuccino::SoundSystem::playSound2D(_hurtSound, _group, Cappuccino::SoundSystem::ChannelType::SoundEffect);
	}
	_shieldTimer = 2.0f;
}

void Enemy::setHurtSound(const std::string& path)
{
	_hurtSound = Cappuccino::SoundSystem::load2DSound(path);
	_group = Cappuccino::SoundSystem::createChannelGroup("hurt");
}

Enemy* Enemy::spawn(Enemy* original, glm::vec3 pos)
{
	Enemy* temp = new Enemy(*original);
	temp->setActive(true);
	temp->_rigidBody._position = pos;
	return temp;
}

RoboGunner::RoboGunner(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs) :
	Enemy(SHADER, textures, meshs)
{
	auto loader = Cappuccino::HitBoxLoader("./Assets/Meshes/Hitboxes/BotBox.obj");

	for (auto x : loader._boxes)
		_rigidBody._hitBoxes.push_back(x);


	_enemyType = "RoboGunner";

	_enemyGun = new AR(*SHADER, std::vector<Cappuccino::Texture*>{}, meshs, "testWeapon", 1.0f, 0.1f, 200, true);

	_enemyGun->setYBulletOffset(1.5f);

	_enemyGun->setShootSound("SentryLaser.wav", "SentryGroup");

	_sound = Cappuccino::SoundSystem::load2DSound("targetAquired.wav");
	_hurtSound = Cappuccino::SoundSystem::load2DSound("machineHurt.wav");
	_group = Cappuccino::SoundSystem::createChannelGroup("robotGroup");
	_hud = new enemyHUD("Robo Gunner");
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
	_enemyType = "Grunt";
	for (auto x : loader._boxes)
		_rigidBody._hitBoxes.push_back(x);


	_enemyGun = new AR(*SHADER, std::vector<Cappuccino::Texture*>{}, meshs, "testWeapon", 1.0f, 0.1f, 200, true);

	_enemyGun->setYBulletOffset(1.5f);

	_enemyGun->setShootSound("SentryLaser.wav", "SentryGroup");

	_sound = Cappuccino::SoundSystem::load2DSound("targetAquired.wav");
	_hurtSound = Cappuccino::SoundSystem::load2DSound("machineHurt.wav");
	_group = Cappuccino::SoundSystem::createChannelGroup("robotGroup");
	_hud = new enemyHUD("Grunt");
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
	_enemyType = "Captain";
	//std::swap(loader._boxes[0], loader._boxes[1]);
	for (auto x : loader._boxes)
		_rigidBody._hitBoxes.push_back(x);


	_enemyGun = new AR(*SHADER, std::vector<Cappuccino::Texture*>{}, meshs, "testWeapon", 1.0f, 0.1f, 200, true);

	_enemyGun->setYBulletOffset(1.5f);

	_enemyGun->setShootSound("SentryLaser.wav", "SentryGroup");

	_sound = Cappuccino::SoundSystem::load2DSound("targetAquired.wav");
	_hurtSound = Cappuccino::SoundSystem::load2DSound("machineHurt.wav");
	_group = Cappuccino::SoundSystem::createChannelGroup("robotGroup");
	_hud = new enemyHUD("Captain");
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
	_enemyType = "Sentry";
	for (auto x : loader._boxes)
		_rigidBody._hitBoxes.push_back(x);


	_enemyGun = new AR(*SHADER, {}, meshes, "testWeapon", 1.0f, 0.1f, 200, true);

	_enemyGun->setShootSound("SentryLaser.wav", "SentryGroup");

	_sound = Cappuccino::SoundSystem::load2DSound("targetAquired.wav");
	_hurtSound = Cappuccino::SoundSystem::load2DSound("machineHurt.wav");
	_group = Cappuccino::SoundSystem::createChannelGroup("robotGroup");
	_hud = new enemyHUD("Sentry");
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

	//_animator.addAnimation(new Cappuccino::Animation(
	//	std::vector<Cappuccino::Mesh*>{ _meshes.back(), testMorph, testMorph1, new Cappuccino::Mesh(*_meshes.back()) },
	//	AnimationType::Idle));
	//_animator.setLoop(AnimationType::Idle, true);

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
	else
	{
		if (!_encountered) {

			Cappuccino::SoundSystem::playSound2D(_sound, _group, Cappuccino::SoundSystem::ChannelType::SoundEffect);
			_encountered = true;
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


	return test1;
}

void Sentry::wander(float dt)
{

	auto norm = glm::normalize(glm::vec3(sinf(glfwGetTime() * 2.0f), -cosf(glfwGetTime() * 2.0f), -1.0f));

	_rigidBody.setVelocity(-norm * 2.5f);
}


Ghoul::Ghoul(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs, const std::optional<float>& mass) :
	Enemy(SHADER, textures, meshs, mass), first(*_meshes.back()), last(*_meshes.back()), frame1("e", "Animations/Crawler/Crawler_kf1.obj"), frame2("ee", "Animations/Crawler/Crawler_kf2.obj"), frame3("eee", "Animations/Crawler/Crawler_kf3.obj")
{
	auto loader = Cappuccino::HitBoxLoader("./Assets/Meshes/Hitboxes/GhoulBox.obj");
	_enemyType = "Ghoul";
	for (auto x : loader._boxes)
		_rigidBody._hitBoxes.push_back(x);

	_enemyGun = new Melee(*SHADER, {}, {}, "e", 1.0f, 1.0f, true);

	_sound = Cappuccino::SoundSystem::load2DSound("ghoulAgro.wav");
	_jumpSound = Cappuccino::SoundSystem::load2DSound("ghoulAgro2.wav");
	_hurtSound = Cappuccino::SoundSystem::load2DSound("ghoulAgro3.wav");
	_group = Cappuccino::SoundSystem::createChannelGroup("ghoulGroup");
	_hud = new enemyHUD("Ghoul");

	_maxHp = 70.0f;
	_hp = _maxHp;
	_maxShield = 0.0f;
	_shield = _maxShield;
	_jump = 3.0f;
	_jumpAnim = 1.0f;
	_distance = 1.0f;
	_weight = 0.5f;

	triggerVolume._size *= 2.0f;

	_meshes.back() = &first;
	_meshes.back()->loadFromData();


	frame1.loadMesh();
	frame2.loadMesh();
	frame3.loadMesh();
	last.loadFromData();

	_animator.addAnimation(new Cappuccino::Animation({
		&first,
		&frame2,
		&frame3,
		&frame3,
		&frame2,
		&frame1,
		&last }, AnimationType::Jump));
	_animator.setLoop(AnimationType::Jump, false);
	_animator.setSpeed(AnimationType::Jump, 5.0f);


	_animator.setAnimationShader(AnimationType::Jump, Cappuccino::Application::_gBufferShader);

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

		if (!_encountered) {

			Cappuccino::SoundSystem::playSound2D(_sound, _group, Cappuccino::SoundSystem::ChannelType::SoundEffect);
			_encountered = true;
		}
		auto newPos = (other->_rigidBody._position /*+ other->_rigidBody._vel/4.0f*/) - _rigidBody._position;
		_camera.lookAt(other->_rigidBody._position);
		auto v = _camera.whereAreWeLooking();

		_transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), -glm::dot(
			glm::normalize(glm::vec3(_transform._transformMat[0].x, _transform._transformMat[0].y, _transform._transformMat[0].z)),
			glm::normalize(glm::vec3(v[2].x, v[2].y, v[2].z))));

		float dist = glm::length(newPos);

		auto normOther = glm::normalize(newPos);

		normOther.y = 0.0f;
		if (_jumpAnim == 1.0f)
		{
			_rigidBody.setVelocity(normOther * 3.0f);
			_jump -= dt;
			alreadyHit = false;
		}
		else {
			_jumpAnim -= dt;
			float attackDist = 5.f;
			if (!_animator.isPlaying(AnimationType::Jump))
				_animator.playAnimation(AnimationType::Jump);

			if (dist <= attackDist && !alreadyHit) {
				other->takeDamage(5.0f);
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
	_enemyType = "Squelch";
	for (auto x : loader._boxes)
		_rigidBody._hitBoxes.push_back(x);

	_enemyGun = new Melee(*SHADER, std::vector<Cappuccino::Texture*>{}, meshs, "testWeapon", 1.0f, 0.1f, 200);

	_enemyGun->setShootSound("SentryLaser.wav", "SentryGroup");

	_sound = Cappuccino::SoundSystem::load2DSound("ghoulAgro3.wav");
	_hurtSound = Cappuccino::SoundSystem::load2DSound("ghoulAgro4.wav");
	_group = Cappuccino::SoundSystem::createChannelGroup("robotGroup");
	_hud = new enemyHUD("Squelch");

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
	_enemyType = "Sentinel";
	for (auto x : loader._boxes)
		_rigidBody._hitBoxes.push_back(x);

	_enemyGun = new AR(*SHADER, {}, {}, "Mega Big Machine Gun", 1.0f, 0.1f, 5, true);

	_enemyGun->setShootSound("bigCannon.wav", "SentryGroup");

	setHurtSound("machineHurt.wav");
	_hud = new enemyHUD("Sentinel");

	float _missileTimer = 5.0f;
	_hp = 1000.0f;

	//_missile = new Missile(_shader, _textures, Cappuccino::MeshLibrary::loadMesh("Sentry", "Sentry.obj"));
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

	_missileTimer -= dt;
	if (_missileTimer <= 0.0f)
	{
		_missileTimer = 5.0f;
		//_missile->setActive(true);
	}
}

Primordial::Primordial(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	:Enemy(SHADER, textures, meshes)
{
	auto loader = Cappuccino::HitBoxLoader("./Assets/Meshes/Hitboxes/GhoulBox.obj");

	for (auto x : loader._boxes)
		_rigidBody._hitBoxes.push_back(x);

	_enemyGun = new AR(*SHADER, {}, {}, "Mega Big Machine Gun", 1.0f, 0.1f, 5, true);

	_enemyGun->setShootSound("bigCannon.wav", "SentryGroup");

	setHurtSound("machineHurt.wav");

	_hud = new enemyHUD("Primordial");
	_enemyType = "Primordial";
	_invuln = false;
	_phases = 0;
	_spawn = 0;

	_hp = _maxHp = 750.0f;
	_shield = _maxShield = 350.0f;
}

void Primordial::hurt(float damage)
{
	if (!_invuln)
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
}

void Primordial::setBabies(Squelch* enemy)
{
	_squelchs.push_back(enemy);
}
void Primordial::setBabies(Ghoul* enemy)
{
	_ghouls.push_back(enemy);
}

void Primordial::release()
{
	if (_spawn > 0) {
		glm::vec3 tempPos(0);
		int random = 0;
		for (int i = _spawn; i > 0; i--)
		{
			random = Cappuccino::randomInt(0, 1);
			tempPos = _rigidBody._position + glm::vec3(Cappuccino::randomFloat(-15.0f, 15.0f), 0.0f, Cappuccino::randomFloat(-15.0f, 15.0f));
			if (random > 0)
			{
				_ghouls[i]->setActive(true);
				_ghouls[i]->setHealth(_ghouls[i]->getMaxHP());
				_ghouls[i]->_rigidBody._position = tempPos;
			}
			else
			{
				_squelchs[i]->setActive(true);
				_squelchs[i]->setHealth(_squelchs[i]->getMaxHP());
				_squelchs[i]->_rigidBody._position = tempPos;
			}
		}
	}
}


void Primordial::invulnCheck()
{
	for (int i = 0; i < _ghouls.size(); i++)
	{
		if (_ghouls[i]->isActive())
			_invuln = true;
		else if (_squelchs[i]->isActive())
			_invuln = true;
	}
}

void Primordial::wander(float dt)
{
}

void Primordial::attack(Class* other, float speed)
{
	if (_phases == 0)
	{
		_phases++;
		_spawn = 3;
	}
	else if (_hp + _shield <= 0.8f * (_maxHp + _maxShield) && _phases == 1)
	{
		_phases++;
		_spawn = 5;
	}
	else if (_hp + _shield <= 0.6f * (_maxHp + _maxShield) && _phases == 2)
	{
		_phases++;
		_spawn = 7;
	}
	else if (_hp + _shield <= 0.4f * (_maxHp + _maxShield) && _phases == 3)
	{
		_phases++;
		_spawn = 9;
	}
	else if (_hp + _shield <= 0.2f * (_maxHp + _maxShield) && _phases == 4)
	{
		_phases++;
		_spawn = 11;
	}
	_invuln = false;
	release();
	invulnCheck();
}

Dino::Dino(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::optional<float>& mass)
	:Enemy(SHADER, textures, meshes, mass)
{
	auto loader = Cappuccino::HitBoxLoader("./Assets/Meshes/Hitboxes/SentryBox.obj");

	for (auto x : loader._boxes)
		_rigidBody._hitBoxes.push_back(x);

	_enemyGun = new AR(*SHADER, {}, {}, "Mega Big Machine Gun", 1.0f, 0.1f, 5, true);

	_enemyGun->setShootSound("bigCannon.wav", "SentryGroup");

	setHurtSound("machineHurt.wav");

	_hud = new enemyHUD("Dino");
}

void Dino::wander(float dt)
{
}

void Dino::attack(Class* other, float speed)
{
}

Missile::Missile(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::optional<float>& mass) : Enemy(SHADER, textures, meshes)
{
	auto loader = Cappuccino::HitBoxLoader("./Assets/Meshes/Hitboxes/SentryBox.obj");
	for (auto x : loader._boxes)
		_rigidBody._hitBoxes.push_back(x);
	_enemyType = "Missile";
	_maxHp = _hp = 1.0f;
	_maxShield = _shield = 0.0f;
}

void Missile::wander(float dt)
{
}

void Missile::attack(Class* other, float speed)
{
	auto newPos = other->_rigidBody._position - _rigidBody._position;

	float dist = glm::length(newPos);

	auto normOther = glm::normalize(newPos);
	auto perp = glm::normalize(cross(other->_rigidBody._position, normOther));

	glm::vec3 crmPos = CatmullRom(speed,
		other->_rigidBody._position - (5.0f * normOther),
		other->_rigidBody._position - (5.0f * perp),
		other->_rigidBody._position + (5.0f * normOther),
		other->_rigidBody._position + (5.0f * perp));

	glm::vec3 dir = glm::normalize(crmPos - _rigidBody._position);

	_rigidBody.setVelocity(dir * 25.0f);

	if (dist <= 2.5f)
	{
		other->takeDamage(2.5f / dist * 150.0f);
		_hp = 0;
	}
}
