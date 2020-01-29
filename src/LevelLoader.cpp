#include "LevelLoader.h"
#include <string>
LevelLoader::LevelLoader(const char* filename)
{
	char tempName[256] ="";
	bool moreFile = true;
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("Failed to open file\n");
		moreFile = false;
	}
		
	
	while (moreFile)
	{
		char line[1024] ="";
		int lineNumber = fscanf(file, "%s", line);
		if (lineNumber == EOF){//if end of file
			moreFile = false;
		}
		else if (strcmp(line, "o") == 0){//if new object
			int errorThing = fscanf(file, "%s\n", &tempName);//get name of object
		}
		else if (strcmp(line, "v") == 0){//if vertex
			glm::vec3 vertex;
			int errorThing = fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
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
				

				_exits.push_back(newDoor);
			}
			else if (tempName[0] == 'D') {
				_entrance._exitBox._size = glm::vec3(1.0f, 4.0f, 4.0f);
				_entrance._exitBox._position = findCenter();
			}				
			else if (tempName[0] == 'L'){
				_lights.push_back(findCenter());
			}
			else if (tempName[0] == 'R'){
				_respawnPoint = findCenter();
			}
			
			_tempVerts.clear();
		}
	}
}

void LevelLoader::rotate(float rotation)
{
	for (unsigned i =0;i<_exits.size();i++)
		_exits[i]._exitBox.rotateBox(rotation);
	_entrance._exitBox.rotateBox(rotation);

	if (rotation / 90.0f == 1.0f){
		_respawnPoint = glm::vec3(_respawnPoint.z, _respawnPoint.y, -_respawnPoint.x);
	}
	else if (rotation / 90.0f == 2.0f){
		_respawnPoint.x *= -1;
		_respawnPoint.z *= -1;
	}
	else if (rotation / 90.0f == 3.0f){
		_respawnPoint = glm::vec3(-_respawnPoint.z, _respawnPoint.y, _respawnPoint.x);
	}

	for (unsigned i = 0; i < _lights.size(); i++) {

		if (rotation / 90.0f == 1.0f) {
			_lights[i] = glm::vec3(_lights[i].z, _lights[i].y, -_lights[i].x);
		}
		else if (rotation / 90.0f == 2.0f) {
			_lights[i].x *= -1;
			_lights[i].z *= -1;
		}
		else if (rotation / 90.0f == 3.0f) {
			_lights[i] = glm::vec3(-_lights[i].z, _lights[i].y, _lights[i].x);
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
