#include "Billboard.h"
#include <Cappuccino/ResourceManager.h>

Billboard::Billboard(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures)
	: GameObject(*SHADER, textures, { Cappuccino::MeshLibrary::loadMesh("Billboard", "Cube2.obj") })
{
	_rigidBody.setGrav(false);
}

void Billboard::childUpdate(float dt)
{
	//_transform.translate(glm::vec3(1.0f, 1.0f, 1.0f) * dt);
}
