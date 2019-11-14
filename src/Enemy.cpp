#include "..\include\Enemy.h"
#include "Cappuccino/CappMath.h"
#include "Cappuccino/HitBoxLoader.h"
#include "Cappuccino/SoundSystem.h"
#include "glm/gtx/rotate_vector.hpp"
#include "Cappuccino/CappMacros.h"

#include "Cappuccino/Input.h"
#include "Cappuccino/Events.h"

Enemy::Enemy(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs, const std::optional<float>& mass)
	:Cappuccino::GameObject(*SHADER, textures, meshs, mass), triggerVolume(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(30.0f, 30.0f, 30.0f)),
	_pLight(glm::vec2(1600.0f, 1200.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f)
{
}

void Enemy::childUpdate(float dt)
{
	_enemyGun->setDelay(dt);
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
	}
	static bool y = false;
	if (isEvent(Events::F) && !y)
		y = true;
	if(y)
		animation->animate(dt);

}

void Enemy::attack(GameObject* other, float speed)
{
	//lerpSpeed = speed;
	//_rigidBody._position = (Cappuccino::Math::lerp(_rigidBody._position, other->_rigidBody._position, lerpFloat));


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

	_rigidBody.setVelocity(normOther * 3.0f);

	auto dot = glm::dot(normOther, _transform.forward);
	auto normForward = glm::normalize(_transform.forward);
	float length = (float)(normOther.length() * normForward.length());

	auto cosine = dot / length;

	auto angle = glm::acos(cosine);


	_transform.forward = glm::rotate(_transform.forward, glm::radians(angle * 10.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	_transform._transformMat[0].x = _transform.forward.x;
	_transform._transformMat[0].y = _transform.forward.y;
	_transform._transformMat[0].z = _transform.forward.z;

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

Sentry::Sentry(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs) : Enemy(&_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{ new Cappuccino::Texture(std::string("matte.png"), Cappuccino::TextureType::DiffuseMap), spec }, std::vector<Cappuccino::Mesh*>{ new Cappuccino::Mesh("Sentry.obj") }, 1.0f)
{

	auto loader = Cappuccino::HitBoxLoader("./Assets/Meshes/Hitboxes/SentryBox.obj");

	for (auto x : loader._boxes)
		_rigidBody._hitBoxes.push_back(x);

	_enemyGun = new AR(*SHADER, std::vector<Cappuccino::Texture*>{}, meshs, "testWeapon", 1.0f, 0.1f, 200);

	_enemyGun->setShootSound("SentryLaser.wav", "SentryGroup");

	_sound = Cappuccino::SoundSystem::load2DSound("targetAquired.wav");
	_hurtSound = Cappuccino::SoundSystem::load2DSound("machineHurt.wav");
	_group = Cappuccino::SoundSystem::createChannelGroup("robotGroup");
	hp = 20.0f;

	auto& m = std::vector<Cappuccino::Mesh*>{ new Cappuccino::Mesh("NUTtest.obj") };
	auto& t = std::vector<Cappuccino::Texture*>{ new Cappuccino::Texture("metal.png",Cappuccino::TextureType::DiffuseMap) };
	for (unsigned i = 0; i < 18; i++)
		_deathParticles.push_back(new Particle(*SHADER, t, m));

	testMorph = new Cappuccino::Mesh("Sentry2.obj");
	testMorph->loadMesh();

	auto testMorph1 = new Cappuccino::Mesh("Sentry3.obj");
	testMorph1->loadMesh();

	animation = new Cappuccino::Animation(std::vector<Cappuccino::Mesh*>{_meshes.back(), testMorph, testMorph1, new Cappuccino::Mesh(*_meshes.back())});

	_rigidBody._position = glm::vec3(26.80f, 1.0f, -59.976f);
	_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.5f);
	Bullet *bullet = new Bullet(_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{ new Cappuccino::Texture(std::string("matte.png"), Cappuccino::TextureType::DiffuseMap), spec }, std::vector<Cappuccino::Mesh*>{ new Cappuccino::Mesh(*mesh) }, glm::vec3(0.0f, 0.0f, 10.0f),
		glm::vec3(0.0f, 0.0f, 0.0f));
	getGun()->addBullets(bullet);

}

Ghoul::Ghoul() : Enemy(&_pLight._pointLightShader, std::vector<Cappuccino::Texture*>{ new Cappuccino::Texture(std::string("matte.png"), Cappuccino::TextureType::DiffuseMap), spec }, std::vector<Cappuccino::Mesh*>{ new Cappuccino::Mesh("Sentry.obj")})
{
	hp = 20.0f;
	_rigidBody._position = glm::vec3(26.80f, 1.0f, -59.976f);
	_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 0.5f);
}
