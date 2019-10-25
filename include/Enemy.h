#pragma once
#include "Cappuccino/GameObject.h"
class Enemy : public Cappuccino::GameObject {
public:
	Enemy(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshs, const std::optional<float>& mass = std::nullopt);
	
	void childUpdate(float dt) override;

	void trackGO(GameObject* other,float speed);
private:
	float lerpFloat = 0.0f;
	float lerpSpeed = 0.01f;

};