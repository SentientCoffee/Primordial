#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <fstream>
#include "Cappuccino/HitBox.h"

struct Door {
	float rotation = 0.0f;
	Cappuccino::HitBox _exitBox = Cappuccino::HitBox(glm::vec3(0.0f),glm::vec3(1.0f));
};

class LevelLoader {
public:
	LevelLoader(const char* filename);
	void rotate(float rotation);

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