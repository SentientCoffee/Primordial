#pragma once
#include "Cappuccino/Player.h"
#include "Cappuccino/Input.h"
#include "UIPointLight.h"
#include "Gun.h"


class UIGun : public Cappuccino::GameObject {
public:
	UIGun(Cappuccino::Shader* SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes);

	void childUpdate(float dt) override;



};

class Commando : public Cappuccino::GameObject {
public:
	Commando(Cappuccino::Shader* SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes);
	
	void childUpdate(float dt) override;

    Cappuccino::CappInput _input;
	Cappuccino::Camera* getCamera() { return _playerCamera;}
	Gun* getGun();
	void toggleGun();
	
private:
	unsigned soundHandle;
	unsigned groupHandle;

	Cappuccino::Shader* _crosshairShader;
	Crosshair* _crosshair;
	UIPointLight _uiLight;
	Cappuccino::Camera* _playerCamera = new Cappuccino::Camera();
	AR* _primary;
	AR* _secondary;
	bool gunToggle;
};