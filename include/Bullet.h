#pragma once
#include "Cappuccino\RigidBody.h"
#include "Cappuccino\GameObject.h"

class Bullet : public Cappuccino::GameObject
{
public:
	Bullet(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs, const glm::vec3& transformPosition, const glm::vec3& origin = glm::vec3(0.0f, 0.0f, 0.0f), const float mass = 1, bool gravity = false);
	void childUpdate(float dt) override;

private:
	Cappuccino::RigidBody _bullet;

};