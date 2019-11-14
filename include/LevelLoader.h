#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <fstream>
#include "Cappuccino/HitBox.h"

struct Door {
	glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f);
	float rotation = 0.0f;
	Cappuccino::HitBox _exitBox = Cappuccino::HitBox(glm::vec3(0.0f),glm::vec3(1.0f));
};

class LevelLoader {
public:
	LevelLoader(const char* filename);


	Door entrance;
	std::vector<Door> exits;
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