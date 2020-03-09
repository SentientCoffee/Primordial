#pragma once
#include "Cappuccino/GameObject.h"
#include "LevelLoader.h"
#include "SpawnLoader.h"
class Building : public Cappuccino::GameObject {
public:
	Building(char* levelData, char* spawnData, char* hitBox, Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs);
	/*
	Purp: This function resets the data for the room so it is no longer rotated
	Pre: None
	Post: None
	*/
	void reset();
	void childUpdate(float dt) override;
	/*
	Purp: This function will rotate the room and all it's data
	Pre: A float representing the degrees rotated (90.0f,180.0f,270.0f)
	Post: None
	*/
	void rotate(float rotation);

	float _currentRotation = 0.0f;//the current rotation of the room
	LevelLoader _levelData;
	SpawnLoader _spawnData;
	unsigned buildingNumber;
private:
	static unsigned numberOfBuildings;
};