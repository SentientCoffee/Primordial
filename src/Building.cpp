#include "..\include\Building.h"
#include "Cappuccino/HitBoxLoader.h"

Building::Building(const Cappuccino::Shader& SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	:GameObject(SHADER, textures, meshes, 1.0f)
{
	Cappuccino::HitBoxLoader loader("./Assets/Meshes/floorHitBox.obj");
	for (unsigned i=0;i<loader._boxes.size();i++)
	{
		_rigidBody._hitBoxes.push_back(loader._boxes[i]);
	}
}

void Building::childUpdate(float dt)
{
	_rigidBody.update(dt,_transform._transformMat);
}
