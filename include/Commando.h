#pragma once
#include "Cappuccino/Player.h"

class Commando : public Cappuccino::Player {
public:
	Commando(const Cappuccino::Shader& SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes);
	
	void childUpdate(float dt) override;

};