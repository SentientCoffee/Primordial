#include "Billboard.h"

Billboard::Billboard(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures)
	:Cappuccino::GameObject(*SHADER,textures,{new Cappuccino::Mesh("Cube2.obj")})
{
	_rigidBody.setGrav(false);
}

void Billboard::childUpdate(float dt)
{
}