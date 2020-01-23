#include "Building.h"
#include "Cappuccino/HitBoxLoader.h"

Building::Building(char* levelData, char* hitBox, Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs)
	:Cappuccino::GameObject(*SHADER, textures, meshs, 1.0f),_levelData(levelData)
{
	auto loader = Cappuccino::HitBoxLoader(hitBox);

	for (auto x : loader._boxes)
		_rigidBody._hitBoxes.push_back(x);
	_rigidBody._position.y = -2.0f;
}

void Building::reset()
{
	while (_currentRotation != 0.0f)
	{
			if (_currentRotation >= 360.0f)
				_currentRotation -= 360.0f;
			else
				rotate(90.0f);
	}
}

void Building::childUpdate(float dt)
{
	
}

void Building::rotate(float rotation)
{
	if (rotation >= 360.0f)
		rotation -= 360.0f;
	std::cout << rotation << std::endl;
	_currentRotation += rotation;
	rotateY(rotation);
	_rigidBody.rotateRigid(rotation);
	_levelData.rotate(rotation);
}
