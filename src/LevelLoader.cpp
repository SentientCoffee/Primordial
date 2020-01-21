#include "LevelLoader.h"
#include <string>
LevelLoader::LevelLoader(const char* filename)
{
	char tempName[256];

	FILE* file = fopen(filename, "r");
	if (file == NULL)
		printf("Failed to open file\n");
	bool moreFile = true;
	while (moreFile)
	{
		char line[1024];
		int lineNumber = fscanf(file, "%s", line);
		if (lineNumber == EOF){//if end of file
			moreFile = false;
		}
		else if (strcmp(line, "o") == 0){//if new object
			fscanf(file, "%s\n", &tempName);//get name of object
		}
		else if (strcmp(line, "v") == 0){//if vertex
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			_tempVerts.push_back(vertex);
		}
		else if (strcmp(line, "s") == 0){
			if (tempName[0] == 'E'){
				Door newDoor;

				std::string rotationString = tempName;
				rotationString = rotationString.substr(rotationString.find_first_of('_')+1,rotationString.find_last_of('_')-5);
				newDoor.rotation = std::stof(rotationString);

				newDoor._exitBox._size = glm::vec3(1.0f, 4.0f, 1.0f);
				newDoor._exitBox._position = findCenter();
				

				exits.push_back(newDoor);
			}
			else if (tempName[0] == 'D') {
				entrance._exitBox._size = glm::vec3(1.0f, 4.0f, 4.0f);
				entrance._exitBox._position = findCenter();
			}				
			else if (tempName[0] == 'L'){
				lights.push_back(findCenter());
			}
			else if (tempName[0] == 'R'){
				_spawnPoint = findCenter();
			}
			
			_tempVerts.clear();
		}
	}
}

void LevelLoader::rotate(float rotation)
{
	for (unsigned i =0;i<exits.size();i++)
		exits[i]._exitBox.rotateBox(rotation);
	entrance._exitBox.rotateBox(rotation);

	if (rotation / 90.0f == 1.0f){
		_spawnPoint = glm::vec3(_spawnPoint.z, _spawnPoint.y, -_spawnPoint.x);
	}
	else if (rotation / 90.0f == 2.0f){
		_spawnPoint.x *= -1;
		_spawnPoint.z *= -1;
	}
	else if (rotation / 90.0f == 3.0f){
		_spawnPoint = glm::vec3(-_spawnPoint.z, _spawnPoint.y, _spawnPoint.x);
	}

	for (unsigned i = 0; i < lights.size(); i++) {

		if (rotation / 90.0f == 1.0f) {
			lights[i] = glm::vec3(lights[i].z, lights[i].y, -lights[i].x);
		}
		else if (rotation / 90.0f == 2.0f) {
			lights[i].x *= -1;
			lights[i].z *= -1;
		}
		else if (rotation / 90.0f == 3.0f) {
			lights[i] = glm::vec3(-lights[i].z, lights[i].y, lights[i].x);
		}
	}
	
}

glm::vec3 LevelLoader::findCenter()
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
		(tempHigh.y / 2 + tempLow.y / 2)-2,
		tempHigh.z / 2 + tempLow.z / 2);
}


unsigned LevelLoader::LightCount = 0;