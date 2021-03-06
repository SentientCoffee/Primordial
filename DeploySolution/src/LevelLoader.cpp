#include "LevelLoader.h"

#include "Cappuccino/CappMacros.h"
#include "Cappuccino/HitBox.h"


#include <Cappuccino/PointLight.h>
#include <fstream>
#include <string>

LevelLoader::LevelLoader(const char* filename)
{
	char tempName[256] = "";
	bool moreFile = true;
	FILE* file = fopen(filename, "r");
	if (file == nullptr) {
		printf("Failed to open file \"%s\"!\n", filename);
		moreFile = false;
	}
		
	
	while (moreFile)
	{
		char line[1024] = "";
		const int lineNumber = fscanf(file, "%s", line);
		if (lineNumber == EOF){//if end of file
			break;
		}

		if (strcmp(line, "o") == 0){//if new object
			int errorThing = fscanf(file, "%s\n", tempName);//get name of object
			CAPP_ASSERT(errorThing, "Failed to read object name");
		}
		else if (strcmp(line, "v") == 0){//if vertex
			glm::vec3 vertex;
			int errorThing = fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			CAPP_ASSERT(errorThing, "Failed to read vertex data");
			_tempVerts.push_back(vertex);
		}
		else if (strcmp(line, "s") == 0 && strcmp(tempName, "") != 0) {
			if (tempName[0] == 'E'){
				DoorLoc newDoor;

				std::string rotationString = tempName;
				rotationString = rotationString.substr(rotationString.find_first_of('_')+1,rotationString.find_last_of('_')-5);
				newDoor._rotation = std::stof(rotationString);

				newDoor._exitBox._size = glm::vec3(2.0f, 4.0f, 1.0f);
				newDoor._exitBox._position = findCenter();
				newDoor._exitBox._position.y += 1;

				_exits.push_back(newDoor);
			}
			else if (tempName[0] == 'D') {
				_entrance._exitBox._size = glm::vec3(2.0f, 4.0f, 4.0f);
				_entrance._exitBox._position = findCenter();
				_entrance._exitBox._position.y += 1;
			}				
			else if (tempName[0] == 'L'){
				_lights.emplace_back(findCenter(), tempName[1] == 'S' ? 1.0f : 0.0f);
			}
			else if (tempName[0] == 'R'){
				_respawnPoint = findCenter();
			}
			else if (tempName[0] == 'M'){
				_shopLocation = findCenter();
			}
			else if (tempName[0] == 'C'){
				chests.push_back(findCenter());
			}
			else if (tempName[0] == 'G')
			{
				_lifts.push_back(GravLift(Cappuccino::HitBox(findCenter(), findBox()), 100.0f));
			}
			else if (tempName[0] == 'H')
			{
				_hurtboxes.push_back(HurtBox(Cappuccino::HitBox(findCenter(), findBox()), 40000.0f));
			}
			else if (tempName[0] == 'T') {
				_teleporterLoc.emplace_back(findCenter());
			}
			
			_tempVerts.clear();
		}
	}
}

void LevelLoader::rotate(float rotation)
{
	for (unsigned i = 0; i < _exits.size(); i++)
		_exits[i]._exitBox.rotateBox(rotation);
	_entrance._exitBox.rotateBox(rotation);

	if (rotation / 90.0f == 1.0f){
		_respawnPoint = glm::vec3(_respawnPoint.z, _respawnPoint.y, -_respawnPoint.x);
		_shopLocation = glm::vec3(_shopLocation.z, _shopLocation.y, -_shopLocation.x);
	}
	else if (rotation / 90.0f == 2.0f){
		_respawnPoint.x *= -1;
		_respawnPoint.z *= -1;

		_shopLocation.x *= -1;
		_shopLocation.z *= -1;
	}
	else if (rotation / 90.0f == 3.0f){
		_respawnPoint = glm::vec3(-_respawnPoint.z, _respawnPoint.y, _respawnPoint.x);
		_shopLocation = glm::vec3(-_shopLocation.z, _shopLocation.y, _shopLocation.x);
	}

	for (unsigned i = 0; i < _lights.size(); i++) {

		if (rotation / 90.0f == 1.0f) {
			_lights[i] = { _lights[i].z, _lights[i].y, -_lights[i].x, _lights[i].w };
		}
		else if (rotation / 90.0f == 2.0f) {
			_lights[i].x *= -1;
			_lights[i].z *= -1;
		}
		else if (rotation / 90.0f == 3.0f) {
			_lights[i] = { -_lights[i].z, _lights[i].y, _lights[i].x, _lights[i].w };
		}
	}
	for (unsigned i = 0; i < chests.size(); i++){
		if (rotation / 90.0f == 1.0f) {
			chests[i] = glm::vec3(chests[i].z, chests[i].y, -chests[i].x);
		}
		else if (rotation / 90.0f == 2.0f) {
			chests[i].x *= -1;
			chests[i].z *= -1;
		}
		else if (rotation / 90.0f == 3.0f) {
			chests[i] = glm::vec3(-chests[i].z, chests[i].y, chests[i].x);
		}
	}
	for (unsigned i = 0; i < _lifts.size(); i++) {
		_lifts[i]._areaOfAffect.rotateBox(rotation);
	}
	for (unsigned i = 0; i < _hurtboxes.size(); i++) {
		_hurtboxes[i]._hurtBox.rotateBox(rotation);
	}
	for (unsigned i = 0; i < _teleporterLoc.size(); i++) {
		if (rotation / 90.0f == 1.0f) {
			_teleporterLoc[i]._position = glm::vec3(_teleporterLoc[i]._position.z, _teleporterLoc[i]._position.y, -_teleporterLoc[i]._position.x);
		}
		else if (rotation / 90.0f == 2.0f) {
			_teleporterLoc[i]._position *= -1;
			_teleporterLoc[i]._position.z *= -1;
		}
		else if (rotation / 90.0f == 3.0f) {
			_teleporterLoc[i]._position = glm::vec3(-_teleporterLoc[i]._position.z, _teleporterLoc[i]._position.y, _teleporterLoc[i]._position.x);
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

glm::vec3 LevelLoader::findBox()
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
	return glm::vec3(tempHigh - tempLow);
}
