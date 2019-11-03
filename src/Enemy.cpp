#include "..\include\Enemy.h"
#include "Cappuccino/CappMath.h"
#include "Cappuccino/HitBoxLoader.h"
#include "Cappuccino/SoundSystem.h"

Enemy::Enemy(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs, const std::optional<float>& mass)
	:Cappuccino::GameObject(*SHADER, textures, meshs, mass), triggerVolume(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(30.0f, 30.0f, 30.0f))
{
	auto loader = Cappuccino::HitBoxLoader("./Assets/Meshes/Hitboxes/SentryBox.obj");

	for (auto x : loader._boxes)
		_rigidBody._hitBoxes.push_back(x);

	enemyGun = new AR(*SHADER, std::vector<Cappuccino::Texture*>{}, meshs, std::string("testWeapon"), 1.0f, 0.1f, 200);

	enemyGun->setShootSound("SentryLaser.wav", "SentryGroup");

	sound = Cappuccino::SoundSystem::load2DSound("targetAquired.wav");
	hurtSound = Cappuccino::SoundSystem::load2DSound("machineHurt.wav");
	group = Cappuccino::SoundSystem::createChannelGroup("robotGroup");
	hp = 20.0f;
}

void Enemy::childUpdate(float dt)
{
	enemyGun->setDelay(dt);
	if (hp <= 0.0f)
		setActive(false);
}

void Enemy::attack(GameObject* other, float speed)
{
	//lerpSpeed = speed;
	//_rigidBody._position = (Cappuccino::Math::lerp(_rigidBody._position, other->_rigidBody._position, lerpFloat));


	static bool first = false;
	if (!targetAquired) {
		first = false;
		_rigidBody.setVelocity(glm::vec3(0.0f));
		wander();
		return;
	}

	if (!first) {
		bool playing;
		Cappuccino::SoundSystem::getChannels()[0]->isPlaying(&playing);
		if (!playing)
			Cappuccino::SoundSystem::playSound2D(sound, group, Cappuccino::SoundSystem::ChannelType::SoundEffect);
		first = true;
	}

	auto newPos = other->_rigidBody._position - _rigidBody._position;

	auto normOther = glm::normalize(newPos);

	_rigidBody.setVelocity(normOther * 3.0f);


	auto norm = glm::normalize(newPos);
	enemyGun->shoot(glm::vec3(norm), _rigidBody._position);
}

void Enemy::wander()
{

	auto norm = glm::normalize(glm::vec3(1.0f, -cosf(glfwGetTime() * 2.0f), sinf(glfwGetTime() * 2.0f)));

	_rigidBody.setVelocity(-norm * 2.5f);
}

void Enemy::hurt(float damage)
{
	hp -= damage;
	Cappuccino::SoundSystem::playSound2D(hurtSound, group, Cappuccino::SoundSystem::ChannelType::SoundEffect);
}
