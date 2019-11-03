#pragma once
#include "Cappuccino/GameObject.h"
#include "Gun.h"

class Enemy : public Cappuccino::GameObject {
public:
	Enemy(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs, const std::optional<float>& mass = std::nullopt);
	
	void childUpdate(float dt) override;

	void attack(GameObject* other,float speed);
	void wander(float dt);


	Gun* getGun() { return enemyGun; }

	void setTrigger(bool yn) { targetAquired = yn; }

	void hurt(float damage);
private:
	unsigned sound = 0, group = 0;
	unsigned hurtSound = 0;
	bool targetAquired = false;

	Gun* enemyGun;
	float lerpFloat = 0.0f;
	float lerpSpeed = 0.01f;

};