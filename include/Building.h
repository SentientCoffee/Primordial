#pragma once
#include "Cappuccino/GameObject.h"
class Building : public Cappuccino::GameObject {
public:
	Building(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs);
	
	void childUpdate(float dt) override;

private:



};