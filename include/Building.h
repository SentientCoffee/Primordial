#pragma once

#include "Cappuccino/GameObject.h"
class Building : public Cappuccino::GameObject {
public:
	Building(const Cappuccino::Shader& SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes);
	void childUpdate(float dt) override;

private:



};