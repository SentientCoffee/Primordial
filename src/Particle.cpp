#include "Particle.h"

Particle::Particle(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	:Cappuccino::GameObject(SHADER,textures,meshes)
{
}

void Particle::childUpdate(float dt)
{
}
