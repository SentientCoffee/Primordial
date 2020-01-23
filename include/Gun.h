#pragma once
#include "Cappuccino/GameObject.h"
#include "Bullet.h"
#include "Cappuccino\Camera.h"

class Enemy;
class Gun : public Cappuccino::GameObject {
public:
	Gun(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes,
		const std::string weapon, const float damage, const float firerate, const int ammo);
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
	unsigned int getMaxAmmo() const { return _ammo; }
	float getDamage() { return _damage; };
	glm::vec3 getOffset() { return _offset; };

	void setShootSound(const std::string& path, const std::string& groupName);

	std::vector<Bullet*>& getBullets() { return _bullets; }

	bool isHitscan()const { return _isHitscan; }

protected:
	Cappuccino::Ray _hitscanRay{ glm::vec3(0.0f),glm::vec3(0.0f) };
	bool _isHitscan = false;

	unsigned soundHandle = 0, groupHandle = 0;

	std::string _weapon = "";
	float _damage = 0;
	float _firerate = 0;
	int _ammo = 0;
	float _delay = 0;
	std::vector<Bullet*> _bullets;
	unsigned int _index = 0;
	unsigned int _ammoCount = 0;
	glm::vec3 _dirVec;
	glm::vec3 _offset;

};

class AR : public Gun {
public:
	AR(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string& weapon, const float damage, const float firerate, const int ammo);
};

//hitscan
class HSAR : public Gun {
public:
	HSAR(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string& weapon, const float damage, const float firerate, const int ammo);
	bool shoot(glm::vec3& camera, glm::vec3& pos) override;
	void addBullets(Bullet* bullet) override;

};

class Pistol : public Gun {
public:
	Pistol(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string& weapon, const float damage, const float firerate, const int ammo);
	bool shoot(glm::vec3& camera, glm::vec3& pos) override;
};

class SG : public Gun {
public:
	SG(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string& weapon, const float damage, const float firerate, const int ammo, const int pellets);

	bool shoot(glm::vec3& camera, glm::vec3& pos) override;
	void addBullets(Bullet* bullet) override;
private:
	int _pellets;
};

class GL : public Gun {
public:
	GL(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string& weapon, const float damage, const float firerate, const int ammo);

	bool shoot(glm::vec3& camera, glm::vec3& pos) override;
	void addBullets(Bullet* bullet) override;

	void specialCollisionBehaviour(const std::vector<Enemy*>& enemies) override;
private:
	Cappuccino::HitBox _aoe;
};

class Melee : public Gun {
public:
	Melee(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string& weapon, const float damage, const float firerate);
	bool shoot(glm::vec3& camera, glm::vec3& pos) override;
	void addBullets(Bullet* bullet) override;
};
