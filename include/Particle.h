#pragma once
#include "Cappuccino/GameObject.h"

class Particle : public Cappuccino::GameObject {
public:
	Particle(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes);

	void childUpdate(float dt);
};