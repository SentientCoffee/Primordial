#pragma once
#include "Cappuccino/GameObject.h"
class Enemy : public Cappuccino::GameObject {
public:
	Enemy(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs, const std::optional<float>& mass = std::nullopt);
	

};