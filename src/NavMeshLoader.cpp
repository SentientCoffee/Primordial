#include "..\include\NavMeshLoader.h"

NavMeshLoader::NavMeshLoader(const char* filename)
{
	char tempName[256] = "";
	bool moreFile = true;
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("Failed to open file\n");
		moreFile = false;
	}

	while (moreFile)
	{
		char line[1024] = "";
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
			if (tempName[0] == 'O') {
				overall = Cappuccino::HitBox(findCenter(), findBox());
			}
			else if (tempName[0] == 'N') {
				_boxes.push_back(Cappuccino::HitBox(findCenter(), findBox()));
			}
		}
	}

	for (unsigned i = 0; i < _boxes.size(); i++) {
		for (int n = _boxes[i]._position.x - _boxes[i]._size.x; n < _boxes[i]._position.x + _boxes[i]._size.x; n++) {
			for (int j = _boxes[i]._position.z - _boxes[i]._size.z; j < _boxes[i]._position.z + _boxes[i]._size.z; j++) {
				bool already = false;
				for (unsigned k = 0; k < navmesh.size(); k++) {
					if (navmesh[k]->_position.x == n && navmesh[k]->_position.z == j)
						already = true;
				}
				if (!already) {
					std::shared_ptr<NavPoint> temp = std::make_shared<NavPoint>();
					temp->_position.x = n;
					temp->_position.z = j;
					navmesh.push_back(temp);
				}
					
			}
		}
	}

	for (unsigned i = 0; i < navmesh.size(); i++) {
		for (unsigned j = 0; j < navmesh.size(); j++) {
			for (int k = -1; k < 2; k++) {
				for (int l = -1; l < 2; l++) {
					if (k == 0 && l == 0)
						continue;
					else{
						if (navmesh[i]->_position.x + k == navmesh[j]->_position.z && navmesh[i]->_position.z + l == navmesh[j]->_position.z) {
							navmesh[i]->_connections.push_back(navmesh[j]);
						}
					}
				}
			}
		}
	}
}

glm::vec3 NavMeshLoader::findCenter()
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

glm::vec3 NavMeshLoader::findBox()
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

void NavMeshLoader::printMesh()
{
	

}


NavPoint::NavPoint(std::shared_ptr<NavPoint> in)
{
	_position = in->_position;
	for (auto x : in->_connections)
		_connections.push_back(x);
}

void NavPoint::reset()
{
	Hcost = -1.0f;
	Gcost = -1.0f;
	Fcost = -1.0f;
	start = false;
	_lastConnection = NULL;

}
