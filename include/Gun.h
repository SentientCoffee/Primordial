#pragma once
#include "Cappuccino/GameObject.h"
#include "Bullet.h"
#include "Cappuccino\Camera.h"

class Gun : public Cappuccino::GameObject {
public:
	Gun(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes,
		const std::string weapon, const float damage, const float firerate, const int ammo);
	Gun(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes);

	void setDelay(float dt);
	bool getFire();

	virtual void addBullets(Bullet* bullet) = 0;
	virtual bool shoot(glm::vec3& camera, glm::vec3& pos) = 0;

	void childUpdate(float dt) override;

	void setDir(glm::vec3& dir) { _dirVec = dir; };
	unsigned int getAmmoCount() const { return _ammo > 0 ? _ammo - _ammoCount : 0; }
	float getDamage() { return _damage; };
	glm::vec3 getOffset() { return _offset; };

	void setShootSound(const std::string& path, const std::string& groupName);

	std::vector<Bullet*>& getBullets() { return _bullets; }
protected:
	unsigned soundHandle = 0, groupHandle = 0;

	std::string _weapon;
	float _damage;
	float _firerate;
	int _ammo;
	float _delay = 0;
	std::vector<Bullet*> _bullets;
	unsigned int _index = 0;
	unsigned int _ammoCount = 0;
	glm::vec3 _dirVec;
	glm::vec3 _offset;

};

class AR : public Gun {
public:
	AR(const Cappuccino::Shader& SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string weapon, const float damage, const float firerate, const int ammo);
	bool shoot(glm::vec3& camera, glm::vec3& pos) override;
	void addBullets(Bullet* bullet) override;
};

class Pistol : public Gun {
public:
	Pistol(const Cappuccino::Shader& SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string weapon, const float damage, const float firerate, const int ammo);
	bool shoot(glm::vec3& camera, glm::vec3& pos) override;
	void addBullets(Bullet* bullet) override;
};

class SG : public Gun {
public:
	SG(const Cappuccino::Shader& SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string weapon, const float damage, const float firerate, const int ammo, const int pellets);

	bool shoot(glm::vec3& camera, glm::vec3& pos) override;
	void addBullets(Bullet* bullet) override;
private:
	int _pellets;
};

class Crosshair : public Gun {
public:
	Crosshair(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes);

	bool shoot(glm::vec3& camera, glm::vec3& pos);
	void addBullets(Bullet* bullet);
};