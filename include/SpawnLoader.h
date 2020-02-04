#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <fstream>
struct SpawnPoint
{
	void rotate(float rotation);
	bool _spawned = false;
	glm::vec3 _position;
};
class SpawnLoader
{
public:
	SpawnLoader(const char* filename);
	void rotate(float rotation);


	float _weight = 0.0f;
	float _usedWeight = 0.0f;
	std::vector <SpawnPoint> _spawnPoints;
private:
	glm::vec3 findCenter();
	std::vector<glm::vec3> _tempVerts;
};