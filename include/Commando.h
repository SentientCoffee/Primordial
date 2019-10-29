#pragma once
#include "Cappuccino/Player.h"
#include "Cappuccino/Input.h"

class Commando : public Cappuccino::GameObject {
public:
	Commando(const Cappuccino::Shader& SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes);
	
	void childUpdate(float dt) override;

    Cappuccino::CappInput _input;
	Cappuccino::Camera* getCamera() { return _playerCamera;}
private:
	Cappuccino::Camera* _playerCamera = new Cappuccino::Camera();

};