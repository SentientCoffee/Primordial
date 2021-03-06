#pragma once
#include "Cappuccino/GameObject.h"
#include "Bullet.h"
#include "Cappuccino\Camera.h"
#include "Cappuccino/SoundSystem.h"

#include "Cappuccino/Random.h"

#include <iostream>

class Enemy;
class Gun : public Cappuccino::GameObject {
public:
	Gun(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes,
		const std::string weapon, const float damage, const float firerate, const int ammo, bool isEnemy = false,float yBulletOffset = 0.0f);
	Gun(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes);

	void setDelay(float dt);
	bool getFire();

	virtual void addBullets(Bullet* bullet);
	virtual bool shoot(glm::vec3& camera, glm::vec3& pos);

	virtual void specialCollisionBehaviour(const std::vector<Enemy*>& enemies) {}

	void childUpdate(float dt) override;

	void setDir(glm::vec3& dir) { _dirVec = dir; };
	unsigned int getAmmoCount() const { return _ammo > 0 ? _ammo - _ammoCount : 0; }
	void setAmmoCount() { _ammoCount = fmaxf(_ammoCount - (_ammo * 0.4f), 0); }
	void setAmmoCount(int ammo) { _ammoCount = ammo; }
	unsigned int getMaxAmmo() const { return _ammo; }
	void setMaxAmmo(int a) { _ammo = a; }
	float getDamage() { return _damage; };
	void setDamage(float dmg) { _damage = dmg; }
	glm::vec3 getOffset() { return _offset; };


	std::vector<Bullet*>& getBullets() { return _bullets; }

	bool isHitscan()const { return _isHitscan; }

	void setYBulletOffset(float offset);
	std::string getWeaponName() { return _weapon; }
protected:
	static Cappuccino::SoundBank* _sounds;

	float _yBulletOffset = 0.0f;

	bool _isEnemy = false;
	Cappuccino::Ray _hitscanRay{ glm::vec3(0.0f),glm::vec3(0.0f) };
	bool _isHitscan = false;


	std::string _weapon = "";
	float _damage = 0;
	float _firerate = 0;
	int _ammo = 0;
	float _delay = 0.0f;
	std::vector<Bullet*> _bullets;
	unsigned int _index = 0;
	unsigned int _ammoCount = 0;
	glm::vec3 _dirVec;
	glm::vec3 _offset;

};

class AR : public Gun {
public:
	AR(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string& weapon, const float damage, const float firerate, const int ammo, bool isEnemy = false);
};

//hitscan
class HSAR : public Gun {
public:
	HSAR(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string& weapon, const float damage, const float firerate, const int ammo,bool isEnemy = false);
	bool shoot(glm::vec3& camera, glm::vec3& pos) override;
	void addBullets(Bullet* bullet) override;
	void specialCollisionBehaviour(const std::vector<Enemy*>& enemies)override;
private:
	float _hitScanDamage = 0.0f;
};

class Pistol : public Gun {
public:
	Pistol(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string& weapon, const float damage, const float firerate, const int ammo,bool isEnemy = false);
	bool shoot(glm::vec3& camera, glm::vec3& pos) override;
};

class SG : public Gun {
public:
	SG(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string& weapon, const float damage, const float firerate, const int ammo, const int pellets, bool isEnemy = false);

	bool shoot(glm::vec3& camera, glm::vec3& pos) override;
	void addBullets(Bullet* bullet) override;
private:
	int _pellets;
};

class GL : public Gun {
public:
	GL(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string& weapon, const float damage, const float firerate, const int ammo, bool isEnemy = false);

	bool shoot(glm::vec3& camera, glm::vec3& pos) override;
	void addBullets(Bullet* bullet) override;

	void specialCollisionBehaviour(const std::vector<Enemy*>& enemies) override;
private:
	Cappuccino::Sound _explosionSound{ "grenadeLauncher-explosivo.wav","GLgroup" };
	Cappuccino::HitBox _aoe;
};

class Melee : public Gun {
public:
	Melee(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string& weapon, const float damage, const float firerate, bool isEnemy);
	bool shoot(glm::vec3& camera, glm::vec3& pos) override;
	void addBullets(Bullet* bullet) override;
};
