#include "SpawnLoader.h"
#include <string>
#include <iostream>
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


SpawnLoader::SpawnLoader(const char* filename)
{

	char tempName[256] = "";

	FILE* file = fopen(filename, "r");
	bool moreFile = true;
	if (file == NULL) 
	{
		printf("Failed to open file\n");
		moreFile = false;
	}
	while (moreFile)
	{
		char line[1024] ="";
		int lineNumber = fscanf(file, "%s", line);
		if (lineNumber == EOF) {//if end of file
			moreFile = false;
		}
		else if (strcmp(line, "o") == 0) {//if new object
			int errorThing = fscanf(file, "%s\n", &tempName);//get name of object
		}
		else if (strcmp(line, "v") == 0) {//if vertex
			glm::vec3 vertex;
			int errorThing = fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			_tempVerts.push_back(vertex);
		}
		else if (strcmp(line, "s") == 0) {
			if (tempName[0] == 'S') {

				SpawnPoint temp;
				temp._position = findCenter();
				//std::cout << _spawnPoints.size()+1<< " x: " << temp._position.x << " y: " << temp._position.y << " z: " << temp._position.z << std::endl;
				_spawnPoints.push_back(temp);
			}
			else if(tempName[0]=='W') { 
				std::string weightString = tempName;
				weightString = weightString.substr(weightString.find_first_of('_') + 1, weightString.find_last_of('_')-2);
				_weight = std::stof(weightString);
				//std::cout << _weight << std::endl;
			}

			_tempVerts.clear();
		}
	}
}

void SpawnLoader::rotate(float rotation)
{
	for (unsigned i = 0; i < _spawnPoints.size(); i++)
		_spawnPoints[i].rotate(rotation);
	_usedWeight = 0.0f;
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


