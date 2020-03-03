#pragma once
#include "Cappuccino/GameObject.h"

class Billboard : public Cappuccino::GameObject {
public:
	Billboard(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures);

	void childUpdate(float dt);
};