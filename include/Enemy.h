#pragma once
#include "Cappuccino/GameObject.h"
#include "Gun.h"
#include "Particle.h"
#include "Cappuccino/AnimationSystem.h"

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
	bool dead();
	const float getWeight() { return _weight; };

	Cappuccino::HitBox triggerVolume;
protected:
	void setHurtSound(const std::string& path);
	std::vector<Particle*> _deathParticles;

	Cappuccino::Animation* animation;

	unsigned _sound = 0, _group = 0;
	unsigned _hurtSound = 0;
	bool _targetAquired = false;

	Gun* _enemyGun;
	float lerpFloat = 0.0f;
	float lerpSpeed = 0.01f;
	float _weight = 0.0f;
};

class Sentry : public Enemy {
public:
	Sentry(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs, const std::optional<float>& mass);

private:
	Cappuccino::Mesh* testMorph;
};

class Ghoul : public Enemy {
public:
	Ghoul(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs, const std::optional<float>& mass = std::nullopt);

	void attack(GameObject* other, float speed);
	void wander();

private:
	float _jump = 2.0f;
	float _jumpAnim = 1.0f;
};

class Sentinel : public Enemy {
public:
	Sentinel(Cappuccino::Shader* SHADER, const std::vector < Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::optional<float>& mass = std::nullopt);

	void wander();
	void attack(GameObject* other, float speed);
};