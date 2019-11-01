#pragma once
#include "Cappuccino\RigidBody.h"
#include "Cappuccino\GameObject.h"

class Bullet : public Cappuccino::GameObject
{
public:
	Bullet();
	Bullet(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs, glm::vec3& pos, glm::vec3& direction);
	void childUpdate(float dt) override;

	float getLife();
	bool lifeState();

	Cappuccino::Shader& getShader() { return _shader; };
	std::vector<Cappuccino::Texture*>& getTextures() { return _textures; };
	std::vector<Cappuccino::Mesh*>& getMeshes() { return _meshes; };

private:
	float _life = 5.0f;
};