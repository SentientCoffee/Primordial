#include "PlayerHUD.h"

#include <Cappuccino/CappMath.h>
#include <Cappuccino/ResourceManager.h>

#include <glm/gtx/transform2.hpp>

using namespace Cappuccino;

enemyHUD::enemyHUD(const std::string& enemyName) {
	if (enemyName == "Sentry")
	{
		_health = _maxHealth = 50;
		_shield = _maxShield = 100;
	}
	else if (enemyName == "Robo Gunner")
	{
		_health = _maxHealth = 200;
		_shield = _maxShield = 200;
	}
	else if (enemyName == "Grunt")
	{
		_health = _maxHealth = 75;
		_shield = _maxShield = 50;
	}
	else if (enemyName == "Captain")
	{
		_health = _maxHealth = 100;
		_shield = _maxShield = 100;
	}
	else if (enemyName == "Ghoul")
	{
		_health = _maxHealth = 70;
		_shield = _maxShield = 0;
	}
	else if (enemyName == "Squelch")
	{
		_health = _maxHealth = 50;
		_shield = _maxShield = 0;
	}
	else if (enemyName == "Primordial")
	{
		_health = _maxHealth = 750;
		_shield = _maxShield = 350;
	}
	else if (enemyName == "Sentinel")
	{
		_health = _maxHealth = 1000;
		_shield = _maxShield = 500;
	}
	else if (enemyName == "Dino")
	{
		_health = _maxHealth = 550;
		_shield = _maxShield = 250;
	}
	else
	{
		_health = _maxHealth = 0;
		_shield = _maxShield = 0;
	}

	if (enemyName == "Dino" || enemyName == "Sentinel" || enemyName == "Primordial")
	{
		_healthBar = new UIBar(glm::vec2(0.0f, 53.5f), // play around with values
			glm::vec4(0.7f, 0.0f, 0.0f, 1.0f),
			glm::vec3(50.0f, 1.5f, 1.0f),
			UIBar::OriginPoint::Middle);
		_shieldBar = new UIBar(glm::vec2(0.0f, 55.0f),
			glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
			glm::vec3(50.0f, 1.0f, 1.0f),
			UIBar::OriginPoint::Middle);

		_healthLerpBG = new UIBar(glm::vec2(0.0f, 53.5f),
			glm::vec4(0.0f, 0.0f, 0.0f, 0.3f),
			glm::vec3(50.0f, 1.5f, 1.0f),
			UIBar::OriginPoint::Middle);
		_shieldLerpBG = new UIBar(glm::vec2(0.0f, 55.0f),
			glm::vec4(0.0f, 0.0f, 0.0f, 0.3f),
			glm::vec3(50.0f, 1.0f, 1.0f),
			UIBar::OriginPoint::Middle);
	}
	else if (enemyName.empty())
	{
		_healthBar = new UIBar(glm::vec2(0.0f, 53.5f), // play around with values
			glm::vec4(0.7f, 0.0f, 0.0f, 0.0f),
			glm::vec3(50.0f, 1.5f, 1.0f),
			UIBar::OriginPoint::Middle);
		_shieldBar = new UIBar(glm::vec2(0.0f, 55.0f),
			glm::vec4(0.0f, 1.0f, 1.0f, 0.0f),
			glm::vec3(50.0f, 1.0f, 1.0f),
			UIBar::OriginPoint::Middle);

		_healthLerpBG = new UIBar(glm::vec2(0.0f, 53.5f),
			glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
			glm::vec3(50.0f, 1.5f, 1.0f),
			UIBar::OriginPoint::Middle);
		_shieldLerpBG = new UIBar(glm::vec2(0.0f, 55.0f),
			glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
			glm::vec3(50.0f, 1.0f, 1.0f),
			UIBar::OriginPoint::Middle);
	}
	else
	{
		_healthBar = new UIBar(glm::vec2(0.0f, 920.0f), // play around with values
			glm::vec4(0.7f, 0.0f, 0.0f, 1.0f),
			glm::vec3(1000.0f, 30.0f, 1.0f),
			UIBar::OriginPoint::Middle);
		_shieldBar = new UIBar(glm::vec2(0.0f, 945.0f),
			glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
			glm::vec3(1000.0f, 20.0f, 1.0f),
			UIBar::OriginPoint::Middle);

		_healthLerpBG = new UIBar(glm::vec2(0.0f, 920.0f),
			glm::vec4(0.0f, 0.0f, 0.0f, 0.3f),
			glm::vec3(1000.0f, 30.0f, 1.0f),
			UIBar::OriginPoint::Middle);
		_shieldLerpBG = new UIBar(glm::vec2(0.0f, 945.0f),
			glm::vec4(0.0f, 0.0f, 0.0f, 0.3f),
			glm::vec3(1000.0f, 20.0f, 1.0f),
			UIBar::OriginPoint::Middle);
	}

	_uiComponents.push_back(_healthLerpBG);
	_uiComponents.push_back(_shieldLerpBG);
	_uiComponents.push_back(_healthBar);
	_uiComponents.push_back(_shieldBar);
}

void enemyHUD::updateHud(float dt) {

	// Lerping bars
	_healthBar->_transform._scaleMat[0].x = static_cast<float>(_health) / static_cast<float>(_maxHealth) * 1000.0f;
	_shieldBar->_transform._scaleMat[0].x = static_cast<float>(_shield) / static_cast<float>(_maxShield) * 1000.0f;

	update(dt);
}

void enemyHUD::toggleHud()
{
	static bool hudOn = false;

	if (hudOn)
		for (auto x : _uiComponents)
			x->setVisible(false);

	else
		for (auto x : _uiComponents)
			x->setVisible(true);

	hudOn ^= true;

}

void enemyHUD::toggleHud(bool yn)
{
	if (!yn)
	{
		for (auto x : _uiComponents)
			x->setVisible(false);
	}
	else
	{
		for (auto x : _uiComponents)
			x->setVisible(true);
	}
}

void enemyHUD::fade(float dt) {
	//if (dt > 0)
	_alpha -= dt;
	//else
	//	_alpha -= dt;
}

HUD::HUD(PlayerClass playerClass) {
	_playerClass = playerClass;
	_currency = 0;

	if (playerClass == PlayerClass::ASSAULT) {
		_health = _maxHealth = 125;
		_shield = _maxShield = 65;
		_ammo   = _maxAmmo   = 72;
		
		_uiComponents.push_back(new UIScreenQuad({
			new Texture("ch-sg",  "crosshair-shotgun.png", TextureType::DiffuseMap),
			new Texture("ch-sgd", "crosshair-shotgun.png", TextureType::DiffuseMap)
		}));
		_uiComponents.push_back(new UIScreenQuad({
			new Texture("ch-p",  "crosshair-pistol.png", TextureType::DiffuseMap),
			new Texture("ch-pd", "crosshair-pistol.png", TextureType::DiffuseMap)
		}));
	}
	else if (playerClass == PlayerClass::COMMANDO) {
		_health = _maxHealth = 100;
		_shield = _maxShield = 50;
		_ammo   = _maxAmmo   = 150;
		
		_uiComponents.push_back(new UIScreenQuad({
			new Texture("ch-ar",  "crosshair-assaultRifle.png", TextureType::DiffuseMap),
			new Texture("ch-ard", "crosshair-assaultRifle.png", TextureType::DiffuseMap)
		}));
		_uiComponents.push_back(new UIScreenQuad({
			new Texture("ch-p",  "crosshair-pistol.png", TextureType::DiffuseMap),
			new Texture("ch-pd", "crosshair-pistol.png", TextureType::DiffuseMap)
		}));
	}
	else if (playerClass == PlayerClass::DEMOLITION) {
		_health = _maxHealth = 110;
		_shield = _maxShield = 60;
		_ammo   = _maxAmmo   = 35;
		
		_uiComponents.push_back(new UIScreenQuad({
			new Texture(std::string("ch-gl"),"crosshair-launcher.png",TextureType::DiffuseMap),
			new Texture(std::string("ch-gl"),"crosshair-launcher.png",TextureType::DiffuseMap)
		}));
		_uiComponents.push_back(new UIScreenQuad({
			new Texture("ch-p",  "crosshair-pistol.png", TextureType::DiffuseMap),
			new Texture("ch-pd", "crosshair-pistol.png", TextureType::DiffuseMap)
		}));
	}
	else if (playerClass == PlayerClass::SCOUT) {
		_health = _maxHealth = 75;
		_shield = _maxShield = 35;
		_ammo   = _maxAmmo   = 100;
		
		_uiComponents.push_back(new UIScreenQuad({
			new Texture("ch-mm",  "crosshair-marksman.png", TextureType::DiffuseMap),
			new Texture("ch-mmd", "crosshair-marksman.png", TextureType::DiffuseMap)
		}));
		_uiComponents.push_back(new UIScreenQuad({
			new Texture("ch-p",  "crosshair-pistol.png", TextureType::DiffuseMap),
			new Texture("ch-pd", "crosshair-pistol.png", TextureType::DiffuseMap)
		}));
	}

	_uiComponents.push_back(new UIScreenQuad({
		new Texture("HUD",      "hud.png",     TextureType::DiffuseMap),
		new Texture("HUD bars", "hudbars.png", TextureType::DiffuseMap)
	}));
	
	_uiComponents.push_back(new UIText("Shields ", { 1600.0f, 1000.0f }, { -1490.0f, -808.0f }, { 1.0f, 1.0f, 1.0f }, 1.0f));
	_uiComponents.push_back(new UIText("HP      ", { 1600.0f, 1000.0f }, { -1344.0f, -926.0f }, { 1.0f, 1.0f, 1.0f }, 1.0f));
	_uiComponents.push_back(new UIText("Ammo    ", { 1600.0f, 1000.0f }, {  1262.0f, -924.0f }, { 1.0f, 1.0f, 1.0f }, 1.0f));
	_uiComponents.push_back(new UIText("Currency", { 1600.0f, 1000.0f }, {  1300.0f,  800.0f }, { 1.0f, 1.0f, 1.0f }, 1.0f));
	
	//Ammo, HP, and shield
	_uiComponents.push_back(new UIBar({  1365.0f, -960.0f }, { 0.0f, 0.5f, 0.0f, 1.0f }, { 475.0f, 100.0f, 1.0f }, UIBar::OriginPoint::BottomRight));
	_uiComponents.push_back(new UIBar({ -1365.0f, -960.0f }, { 0.7f, 0.0f, 0.0f, 1.0f }, {  475.0f, 100.0f, 1.0f }, UIBar::OriginPoint::BottomLeft));
	_uiComponents.push_back(new UIBar({ -1510.0f, -855.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, {  560.0f, 110.0f, 1.0f }, UIBar::OriginPoint::BottomLeft));

	//Shearing bars
	auto ammoBar = dynamic_cast<UIBar*>(_uiComponents[7]);
	auto healthBar = dynamic_cast<UIBar*>(_uiComponents[8]);
	auto shieldBar = dynamic_cast<UIBar*>(_uiComponents[9]);

	ammoBar->_transform._translateMat[1].x = 0.58f;
	ammoBar->_transform._translateMat[2].x = 0.58f;

	healthBar->_transform._translateMat[1].x = -0.58f;
	healthBar->_transform._translateMat[2].x = -0.58f;

	shieldBar->_transform._translateMat[1].x = -0.58f;
	shieldBar->_transform._translateMat[2].x = -0.58f;

	// Pause
	_uiComponents.push_back(new UIBar(glm::vec2(-25.0f, -175.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.2f), glm::vec3(510.0f, 850.0f, 1.0f), UIBar::OriginPoint::Middle));
	_uiComponents.back()->setVisible(false);
	_uiComponents.push_back(new UIBar(glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 0.2f), glm::vec3(3200.0f, 2000.0f, 1.0f), UIBar::OriginPoint::Middle));
	_uiComponents.back()->setVisible(false);

	//auto t = static_cast<UIBar*>(_uiComponents.back());
	//t->_transform.scale(glm::vec3(10.0f, 1.0f, 1.0f), 1.0f);
	//t->_transform.update();
	//t->_transform._scaleMat = glm::shearX3D(t->_transform._scaleMat, 1.0f,-1.0f);
}

void HUD::setHealth(const unsigned int hp)        { _health   = hp; }
void HUD::setShield(const unsigned int sp)        { _shield   = sp; }
void HUD::setAmmoCount(const unsigned int ac)     { _ammo     = ac; }
void HUD::setAmmoMax(const unsigned int am)       { _maxAmmo  = am; }
void HUD::setCurrencyCount(const unsigned int cp) { _currency = cp; }

void HUD::updateHud(float dt) {
	auto shieldText   = dynamic_cast<UIText*>(_uiComponents[3]);
	auto hpText       = dynamic_cast<UIText*>(_uiComponents[4]);
	auto ammoText     = dynamic_cast<UIText*>(_uiComponents[5]);
	auto currencyText = dynamic_cast<UIText*>(_uiComponents[6]);
	
	auto ammoBar   = dynamic_cast<UIBar*>(_uiComponents[7]);
	auto healthBar = dynamic_cast<UIBar*>(_uiComponents[8]);
	auto shieldBar = dynamic_cast<UIBar*>(_uiComponents[9]);
	
	shieldText->  setText(std::to_string(_shield));
	hpText->      setText(std::to_string(_health));
	ammoText->    setText(std::to_string(_ammo));
	currencyText->setText(std::to_string(_currency));

	ammoBar->  _transform._scaleMat[0].x = static_cast<float>(_ammo)   / static_cast<float>(_maxAmmo)   * 475.0f;
	healthBar->_transform._scaleMat[0].x = static_cast<float>(_health) / static_cast<float>(_maxHealth) * 475.0f;
	shieldBar->_transform._scaleMat[0].x = static_cast<float>(_shield) / static_cast<float>(_maxShield) * 560.0f;

	update(dt);
}

void HUD::toggleCrosshair(bool yn)
{
	if (yn)
	{
		_uiComponents[0]->setVisible(true);
		_uiComponents[1]->setVisible(false);
	}
	else
	{
		_uiComponents[0]->setVisible(false);
		_uiComponents[1]->setVisible(true);
	}
}

void HUD::disableCrosshair()
{
	_uiComponents[0]->setVisible(false);
	_uiComponents[1]->setVisible(false);
}

void HUD::toggleHud()
{
	static bool hudOn = true;

	if (hudOn)
		for (auto x : _uiComponents)
			x->setVisible(false);

	else
		for (auto x : _uiComponents)
			x->setVisible(true);

	hudOn = !hudOn;

	auto pauseBar = static_cast<Cappuccino::UIBar*>(_uiComponents[10]);
	pauseBar->setVisible(false);
	auto menuBar = static_cast<Cappuccino::UIBar*>(_uiComponents[11]);
	menuBar->setVisible(false);

}


void HUD::toggleHud(bool yn)
{
	for (auto x : _uiComponents)
		x->setVisible(yn);

	auto pauseBar = static_cast<Cappuccino::UIBar*>(_uiComponents[10]);
	pauseBar->setVisible(false);
	auto menuBar = static_cast<Cappuccino::UIBar*>(_uiComponents[11]);
	menuBar->setVisible(false);
}

void HUD::togglePauseScreen()
{
	auto pauseBar = static_cast<Cappuccino::UIBar*>(_uiComponents[10]);
	pauseBar->setVisible(!pauseBar->isVisible());
	auto menuBar = static_cast<Cappuccino::UIBar*>(_uiComponents[11]);
	menuBar->setVisible(!menuBar->isVisible());
}