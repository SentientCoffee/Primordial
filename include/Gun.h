#pragma once
#include "Cappuccino/GameObject.h"
#include "Bullet.h"

class Gun : public Cappuccino::GameObject {
public:
	Gun(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs, std::string& weapon, float& damage, float& firerate);

	bool getFire(float dt);
	void addBullets(Bullet bullet);

private:
	std::string _weapon;
	float _damage;
	float _firerate;
	bool _fire;
	std::vector<Bullet> _bullets;

};