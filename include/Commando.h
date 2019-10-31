#pragma once
#include "Cappuccino/Player.h"
#include "Cappuccino/Input.h"
#include "UIPointLight.h"


class UIGun : public Cappuccino::GameObject {
public:
	UIGun(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes);

	void childUpdate(float dt) override;
	glm::vec3 _dirVec;

};

class Commando : public Cappuccino::GameObject {
public:
	Commando(Cappuccino::Shader* SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes);
	
	void childUpdate(float dt) override;

    Cappuccino::CappInput _input;
	Cappuccino::Camera* getCamera() { return _playerCamera;}
private:
	UIPointLight _uiLight;
	UIGun* _uiGun;
	Cappuccino::Camera* _playerCamera = new Cappuccino::Camera();

};