#pragma once 
#include "Cappuccino/CappInput.h"
#include "UIPointLight.h"
#include "Gun.h"
#include "PlayerHUD.h"

class Class : public Cappuccino::GameObject {
public:
	Class(Cappuccino::Shader* SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes);

	void childUpdate(float dt) override;

	Cappuccino::CappInput _input;
	Cappuccino::Camera* getCamera() { return _playerCamera; }
	Gun* getGun();
	void addAmmo(Bullet* primary, Bullet* secondary );
	void toggleGun(const bool gun);

	void setActive(bool yn);
	UIPointLight& getUILight() { return _uiLight; }
protected:
	unsigned soundHandle;
	unsigned groupHandle;
	HUD* _hud;

	Cappuccino::Shader* _crosshairShader;
	Gun* _crosshair;
	UIPointLight _uiLight;
	Cappuccino::Camera* _playerCamera = new Cappuccino::Camera();
	Gun* _primary;
	Pistol* _secondary;
	bool gunToggle;
};

class Commando : public Class {
public:
	Commando(Cappuccino::Shader* SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes);
};

class Assault : public Class {
public:
	Assault(Cappuccino::Shader* SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes);
};

class Scout : public Class {
public:
	Scout(Cappuccino::Shader* SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes);
};

class Demolitionist : public Class {
public:
	Demolitionist(Cappuccino::Shader* SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes);
};