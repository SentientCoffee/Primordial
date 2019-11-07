#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <fstream>
class LevelLoader {
public:
	LevelLoader(const char* filename);


	glm::vec3 entrance;
	std::vector<glm::vec3> exits;
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