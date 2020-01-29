#include "SpawnLoader.h"

void SpawnPoint::rotate(float rotation)
{
	if (rotation / 90.0f == 1.0f) {
		_position = glm::vec3(_position.z, _position.y, -_position.x);
	}
	else if (rotation / 90.0f == 2.0f) {
		_position.x *= -1;
		_position.z *= -1;
	}
	else if (rotation / 90.0f == 3.0f) {
		_position = glm::vec3(-_position.z, _position.y, _position.x);
	}
	_spawned = false;
}

void SpawnArea::rotate(float rotation)
{
	for (unsigned i = 0; i < _points.size(); i++)
		_points[i].rotate(rotation);
}

SpawnLoader::SpawnLoader(const char* filename)
{

}

void SpawnLoader::rotate(float rotation)
{
	for (unsigned i = 0; i < _spawnAreas.size(); i++)
		_spawnAreas[i].rotate(rotation);
}

glm::vec3 SpawnLoader::findCenter()
{
	glm::vec3 tempHigh = _tempVerts[0];
	glm::vec3 tempLow = _tempVerts[0];
	for (unsigned int i = 0; i < _tempVerts.size(); i++)
	{
		if (_tempVerts[i].x > tempHigh.x)
			tempHigh.x = _tempVerts[i].x;
		if (_tempVerts[i].y > tempHigh.y)
			tempHigh.y = _tempVerts[i].y;
		if (_tempVerts[i].z > tempHigh.z)
			tempHigh.z = _tempVerts[i].z;

		if (_tempVerts[i].x < tempLow.x)
			tempLow.x = _tempVerts[i].x;
		if (_tempVerts[i].y < tempLow.y)
			tempLow.y = _tempVerts[i].y;
		if (_tempVerts[i].z < tempLow.z)
			tempLow.z = _tempVerts[i].z;
	}

	return glm::vec3(tempHigh.x / 2 + tempLow.x / 2,
		(tempHigh.y / 2 + tempLow.y / 2) - 2,
		tempHigh.z / 2 + tempLow.z / 2);
}


