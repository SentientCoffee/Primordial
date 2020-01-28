#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <fstream>
struct SpawnPoint
{
	bool _spawned = false;
	glm::vec3 _position;
};
struct SpawnArea
{
	float _weight = 0.0f;
	float _usedWeight = 0.0f;
	std::vector <SpawnPoint> _points;
};
class SpawnLoader
{
public:


private:


};