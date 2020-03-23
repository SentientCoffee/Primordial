#pragma once
#include "Cappuccino/GameObject.h"
#include "Gun.h"
#include "Particle.h"
#include "Class.h"
#include "Cappuccino/AnimationSystem.h"
#include <msxml.h>

#include "StateMachine.h"

class Class;
class Enemy : public Cappuccino::GameObject {
public:
	Enemy(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs, const std::optional<float>& mass = std::nullopt);
	void childUpdate(float dt) override;

	virtual void attack(Class* other, float dt);
	virtual void wander(float dt);


	Gun* getGun() { return _enemyGun; }

	void setTrigger(bool yn) { _targetAquired = yn; }
	bool isTriggered() const { return _targetAquired; }

	Enemy* spawn(Enemy* original, glm::vec3 pos);

	glm::vec3 CatmullRom(float t, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);

	void setHealth(const float hp) { _hp = hp; }
	void setShield(const float shield) { _shield = shield; }
	float getMaxHP() { return _maxHp; }
	float getMaxShield() { return _maxShield; }
	float getHP() { return _hp; }
	float getShield() { return _shield; }
	void hurt(float damage);
	bool dead();
	const float getWeight() { return _weight; };
	enemyHUD* getHUD() { return _hud; };

	Cappuccino::HitBox triggerVolume;

	std::string _enemyType = "";

	void resetEnemy();
	StateMachine _stateMachine;
protected:
	static std::vector<Cappuccino::SoundBank*> _sounds;

	Cappuccino::Camera _camera;//used to create a view matrix that rotates the enemy in the proper direction

	float _shieldTimer = 0.0f;

	void setHurtSound(const std::string& path);
	std::vector<Particle*> _deathParticles;

	unsigned _sound = 0, _group = 0;
	unsigned _hurtSound = 0;
	bool _targetAquired = false;
	bool _encountered = false;
	enemyHUD* _hud = new enemyHUD("");


	float _hp;
	float _maxHp;
	float _shield;
	float _maxShield;
	float _speed;

	Gun* _enemyGun = nullptr;
	float lerpFloat = 0.0f;
	float lerpSpeed = 0.01f;
	float _weight = 0.0f;
	float _distance = 0.0f;
	float _wanderCycle = 10.0f;
};

class Sentry : public Enemy {
public:
	Sentry(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::optional<float>& mass);

	void attack(Class* other, float speed) override;
	void wander(float dt) override;

private:
	Cappuccino::Mesh* testMorph;
};

class RoboGunner : public Enemy {
public:
	RoboGunner(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs);

private:
	Cappuccino::Mesh first;
	Cappuccino::Mesh frame1;
	Cappuccino::Mesh frame2;
	Cappuccino::Mesh last;

	Cappuccino::Mesh wMesh1;
	Cappuccino::Mesh wMesh2;
	Cappuccino::Mesh wMesh3;
	Cappuccino::Mesh wMesh4;
	Cappuccino::Mesh wMesh5;
	Cappuccino::Mesh wMesh6;
	Cappuccino::Mesh wMesh7;

};

class Grunt : public Enemy {
public:
	Grunt(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs);

};

class Captain : public Enemy {
public:
	Captain(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs);

};

class Ghoul : public Enemy {
public:
	Ghoul(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs, const std::optional<float>& mass = std::nullopt);

	void attack(Class* other, float speed);
	void wander(float dt);

private:
	bool alreadyHit = false;
	Cappuccino::Mesh first;
	Cappuccino::Mesh frame1;
	Cappuccino::Mesh frame2;
	Cappuccino::Mesh frame3;
	Cappuccino::Mesh last;

	Cappuccino::Mesh wMesh1;
	Cappuccino::Mesh wMesh2;
	Cappuccino::Mesh wMesh3;
	Cappuccino::Mesh wMesh4;
	Cappuccino::Mesh wMesh5;
	Cappuccino::Mesh wMesh6;
	Cappuccino::Mesh wMesh7;
	Cappuccino::Mesh wMesh8;
	Cappuccino::Mesh wMesh9;

	unsigned _jumpSound = 0;
	float _jump = 2.0f;
	float _jumpAnim = 1.0f;
};

class Squelch : public Enemy {
public:
	Squelch(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs);

	void attack(Class* other, float speed);
	void wander(float dt);

private:
	Cappuccino::Mesh first;
	Cappuccino::Mesh frame1;
	Cappuccino::Mesh frame2;
	Cappuccino::Mesh frame3;
	Cappuccino::Mesh frame4;
	Cappuccino::Mesh frame5;
	Cappuccino::Mesh frame6;
	Cappuccino::Mesh frame7;
	Cappuccino::Mesh frame8;
	Cappuccino::Mesh last;

	Cappuccino::Mesh wMesh1;
	Cappuccino::Mesh wMesh2;
	Cappuccino::Mesh wMesh3;
	Cappuccino::Mesh wMesh4;
	Cappuccino::Mesh wMesh5;
	Cappuccino::Mesh wMesh6;
	Cappuccino::Mesh wMesh7;
	Cappuccino::Mesh wMesh8;
	Cappuccino::Mesh wMesh9;

	float _timer = 1.0f;
	bool _primed = false;
};

class Sentinel : public Enemy {
public:
	Sentinel(Cappuccino::Shader* SHADER, const std::vector < Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::optional<float>& mass = std::nullopt);

	void wander(float dt);
	void attack(Class* other, float speed);

private:
	float _missileTimer = 5.0f;
	//Missile* _missile;
};

class Primordial : public Enemy {
public:
	Primordial(Cappuccino::Shader* SHADER, const std::vector < Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes);

	void wander(float dt);
	void attack(Class* other, float speed);
	void hurt(float damage);

	void setBabies(Squelch* enemy);
	void setBabies(Ghoul* enemy);
	void release();
	void invulnCheck();


private:
	unsigned int _phases;
	unsigned int _spawn;
	std::vector<Ghoul*> _ghouls;
	std::vector<Squelch*> _squelchs;
	bool _invuln;
};

class Dino : public Enemy {
public:
	Dino(Cappuccino::Shader* SHADER, const std::vector < Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::optional<float>& mass = std::nullopt);

	void wander(float dt);
	void attack(Class* other, float speed);
};

class Missile : public Enemy {
public:
	Missile(Cappuccino::Shader* SHADER, const std::vector < Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::optional<float>& mass = std::nullopt);

	void wander(float dt);
	void attack(Class* other, float speed);
};