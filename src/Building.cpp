#include "..\include\Building.h"
#include "Cappuccino/HitBoxLoader.h"

Building::Building(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs)
	:Cappuccino::GameObject(*SHADER, textures, meshs, 1.0f)
{
	_rigidBody._hitBoxes.push_back(Cappuccino::HitBox(glm::vec3(0, 0, 0), glm::vec3(10.f,0.2f,10.0f)));
	_rigidBody._position.y = -2.0f;
	_rigidBody.setGrav(false);
	
	//Cappuccino::HitBoxLoader loader("./Assets/Meshes/floorHitBox.obj");
	//for (unsigned i=0;i<loader._boxes.size();i++)
	//{
	//	_rigidBody._hitBoxes.push_back(loader._boxes[i]);
	//}
}

void Building::childUpdate(float dt)
{
	
}
