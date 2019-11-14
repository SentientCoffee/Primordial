#pragma once
#include "Cappuccino/GameObject.h"
#include "Gun.h"
#include "Particle.h"
#include "Cappuccino/AnimationSystem.h"
#include "UIPointLight.h"

class Enemy : public Cappuccino::GameObject {
public:
	Enemy(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs, const std::optional<float>& mass = std::nullopt);

	void childUpdate(float dt) override;

	virtual void attack(GameObject* other, float speed);
	virtual void wander();


	Gun* getGun() { return _enemyGun; }

	void setTrigger(bool yn) { _targetAquired = yn; }
	bool isTriggered() const { return _targetAquired; }

	void hurt(float damage);
	Cappuccino::HitBox triggerVolume;
protected:
	std::vector<Particle*> _deathParticles;
	Cappuccino::PointLight _pLight;
	Cappuccino::Mesh* testMorph;
	Cappuccino::Animation* animation;

	unsigned _sound = 0, _group = 0;
	unsigned _hurtSound = 0;
	bool _targetAquired = false;

	Gun* _enemyGun;
	float lerpFloat = 0.0f;
	float lerpSpeed = 0.01f;


	Cappuccino::Texture *diffuse = new Cappuccino::Texture(std::string("metal.png"), Cappuccino::TextureType::DiffuseMap);
	Cappuccino::Texture *spec = new Cappuccino::Texture(std::string("metal.png"), Cappuccino::TextureType::SpecularMap);
	Cappuccino::Mesh *mesh = new Cappuccino::Mesh("Bullet.obj");
};

class Sentry : public Enemy {
public:
	Sentry(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs);

private:

};

class Ghoul : public Enemy {
public:
	Ghoul();

	void attack(GameObject* other, float speed);
	void wander();

private:

};