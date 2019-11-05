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
	
};