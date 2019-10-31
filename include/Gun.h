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
	void shoot(glm::vec3 camera, glm::vec3 pos);

	void childUpdate(float dt) override;

	void setDir(glm::vec3 dir) { dirVec = dir; };
	float getDamage() { return _damage; };
	bool getAmmoState() { return _ammoState; };


private:
	std::string _weapon;
	float _damage;
	float _firerate;
	int _ammo;
	bool _ammoState;
	float _delay = 0;
	std::vector<Bullet*> _bullets;
	unsigned int index = 0;
	glm::vec3 dirVec;

};