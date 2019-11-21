#pragma once
#include "Cappuccino/GameObject.h"
#include "LevelLoader.h"
class Building : public Cappuccino::GameObject {
public:
	Building(char* levelData,char* hitBox, Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs);
	
	void childUpdate(float dt) override;
	LevelLoader _levelData;
	void rotate(float rotation);
	float _currentRotation = 0.0f;
private:
	


};