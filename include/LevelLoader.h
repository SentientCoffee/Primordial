#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <fstream>
#include "Cappuccino/HitBox.h"
#include "Cappuccino/CappMacros.h"

struct Door {
	float rotation = 0.0f;
	Cappuccino::HitBox _exitBox = Cappuccino::HitBox(glm::vec3(0.0f),glm::vec3(1.0f));
};

class LevelLoader {
public:
	LevelLoader(const char* filename);
	/*
	Purp: 
	Pre: 
	Post: 
	*/
	void rotate(float rotation);

	Door _entrance;
	std::vector<Door> _exits;
	std::vector<glm::vec3> _lights;
	glm::vec3 _respawnPoint;
	glm::vec3 _shopLocation;
protected:
private:
	/*
	Purp:This function will find the center of the given verts
	Pre: None
	Post: A vec3 of the center of the given verts
	*/
	glm::vec3 findCenter();
	std::vector <glm::vec3> _tempVerts;
};