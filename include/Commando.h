#pragma once
#include "Cappuccino/Player.h"
#include "Cappuccino/Input.h"
#include "UIPointLight.h"
#include "Gun.h"

class Commando : public Cappuccino::GameObject {
public:
	Commando(Cappuccino::Shader* SHADER, std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes);

	void childUpdate(float dt) override;

	Cappuccino::CappInput _input;
	Cappuccino::Camera* getCamera() { return _playerCamera; }
	Gun* getGun();
	void addAmmo(Bullet* primary, Bullet* secondary );
	void toggleGun(const bool gun);

	void setActive(bool yn);
	UIPointLight& getUILight() { return _uiLight; }
private:
	unsigned soundHandle;
	unsigned groupHandle;

	Cappuccino::Shader* _crosshairShader;
	Crosshair* _crosshair;
	UIPointLight _uiLight;
	Cappuccino::Camera* _playerCamera = new Cappuccino::Camera();
	AR* _primary;
	Pistol* _secondary;
	bool gunToggle;
};