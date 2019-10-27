#pragma once
#include "Cappuccino\RigidBody.h"
#include "Cappuccino\GameObject.h"

class Bullet : public Cappuccino::GameObject
{
public:
	Bullet(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs, glm::vec3& pos, glm::vec3& direction);

	void childUpdate(float dt) override;

	bool getLife();

private:
	float _life;
};