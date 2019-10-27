#pragma once
#include "Cappuccino/GameObject.h"
#include "Bullet.h"

class Gun : public Cappuccino::GameObject {
public:
	Gun(const Cappuccino::Shader& SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, const std::string weapon, const float damage, const float firerate, const int ammo);

	bool getFire(float dt);
	void addBullets(glm::vec3& pos, glm::vec3& direction);

	void childUpdate(float dt) override;

private:
	std::string _weapon;
	float _damage;
	float _firerate;
	int _ammo;
	bool _fire;
	std::vector<Bullet> _bullets;

};