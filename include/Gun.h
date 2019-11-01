#pragma once
#include "Cappuccino/GameObject.h"
#include "Bullet.h"
#include "Cappuccino\Camera.h"

class Gun : public Cappuccino::GameObject {
public:
	Gun(const Cappuccino::Shader& SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string weapon, const float damage, const float firerate, const int ammo);

	void setDelay(float dt);
	bool getFire();
	void addBullets(Bullet* bullet);
	void shoot(glm::vec3& camera, glm::vec3& pos);

	void childUpdate(float dt) override;

	void setDir(glm::vec3& dir) { _dirVec = dir; };
	float getDamage() { return _damage; };

	void setShootSound(const std::string& path, const std::string& groupName);

private:
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

};