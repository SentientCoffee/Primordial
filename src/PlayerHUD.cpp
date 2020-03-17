#include "PlayerHUD.h"
#include "Cappuccino/CappMath.h"
#include "Cappuccino/ResourceManager.h"
#include "glm/gtx/transform2.hpp"

using namespace Cappuccino;

enemyHUD::enemyHUD(std::string enemyName) {
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
	else if (enemyName == "")
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
		_healthBar = new UIBar(glm::vec2(0.0f, 1070.0f), // play around with values
			glm::vec4(0.7f, 0.0f, 0.0f, 1.0f),
			glm::vec3(1000.0f, 30.0f, 1.0f),
			UIBar::OriginPoint::Middle);
		_shieldBar = new UIBar(glm::vec2(0.0f, 1100.0f),
			glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
			glm::vec3(1000.0f, 20.0f, 1.0f),
			UIBar::OriginPoint::Middle);

		_healthLerpBG = new UIBar(glm::vec2(0.0f, 1070.0f),
			glm::vec4(0.0f, 0.0f, 0.0f, 0.3f),
			glm::vec3(1000.0f, 30.0f, 1.0f),
			UIBar::OriginPoint::Middle);
		_shieldLerpBG = new UIBar(glm::vec2(0.0f, 1100.0f),
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
	_healthBar->_transform._scaleMat[0].x = ((float)_health / (float)_maxHealth) * 50.0f;
	_shieldBar->_transform._scaleMat[0].x = ((float)_shield / (float)_maxShield) * 50.0f;

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
	if (dt > 0)
		_alpha -= dt;
	else
		_alpha -= dt;
}

HUD::HUD(PlayerClass playerClass) {
	_playerClass = playerClass;
	_currency = 0;

	if (playerClass == PlayerClass::ASSAULT) {
		_colour = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
		_health = _maxHealth = 125;
		_shield = _maxShield = 65;
		_ammo = _maxAmmo = 72;
		_class = "A S S A U L T";
	}
	else if (playerClass == PlayerClass::COMMANDO) {
		_colour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		_health = _maxHealth = 100;
		_shield = _maxShield = 50;
		_ammo = _maxAmmo = 150;
		_class = "C O M M A N D O";
	}
	else if (playerClass == PlayerClass::DEMOLITION) {
		_colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		_health = _maxHealth = 110;
		_shield = _maxShield = 60;
		_ammo = _maxAmmo = 35;
		_class = "D E M O L I T I O N I S T";
	}
	else if (playerClass == PlayerClass::SCOUT) {
		_colour = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		_health = _maxHealth = 75;
		_shield = _maxShield = 35;
		_ammo = _maxAmmo = 100;
		_class = "S C O U T";
	}

	using namespace Cappuccino;
	_uiComponents.push_back(new UIScreenQuad({ new Texture(std::string("E"),"hud.png",TextureType::DiffuseMap),
		new Texture(std::string("ee"),"hudbars.png",TextureType::DiffuseMap) }));
	_uiComponents.push_back(new UIText("Shields", { 1600.0f,1000.0f }, 2.0f * glm::vec2(-745.0f, -404.0f), { 1.0f,1.0f,1.0f }, 1.0f));
	_uiComponents.push_back(new UIText("HP", { 1600.0f,1000.0f }, 2.0f * glm::vec2(-672.0f, -463.0f), { 1.0f,1.0f,1.0f }, 1.0f));
	_uiComponents.push_back(new UIText("Ammo", { 1600.0f,1000.0f }, 2.0f * glm::vec2(631.0f, -462.0f), { 1.0f,1.0f,1.0f }, 1.0f));
	_uiComponents.push_back(new UIBar(glm::vec2(687.0f,-476.0f)*2.0f, glm::vec4(0.0f, 0.5f, 0.0f, 1.0f), glm::vec3(350.0f * 10.0f, 100.0f, 1.0f), UIBar::OriginPoint::BottomRight));

	auto t = static_cast<UIBar*>(_uiComponents.back());
	//t->_transform.scale(glm::vec3(10.0f, 1.0f, 1.0f), 1.0f);
	//t->_transform.update();
	//t->_transform._scaleMat = glm::shearX3D(t->_transform._scaleMat, 1.0f,-1.0f);
}

void HUD::setHealth(unsigned int hp) { _health = hp; }
void HUD::setShield(unsigned int sp) { _shield = sp; }
void HUD::setAmmoCount(unsigned int ac) { _ammo = ac; }
void HUD::setAmmoMax(unsigned int am) { _maxAmmo = am; }
void HUD::setCurrencyCount(unsigned int cp) { _currency = cp; }

void HUD::updateHud(float dt) {

	auto shieldText = static_cast<Cappuccino::UIText*>(_uiComponents[1]);
	auto hpText = static_cast<Cappuccino::UIText*>(_uiComponents[2]);
	auto ammoText = static_cast<Cappuccino::UIText*>(_uiComponents[3]);
	shieldText->setText(std::to_string(_shield));
	hpText->setText(std::to_string(_health));
	ammoText->setText(std::to_string(_ammo));

	auto ammoBar = static_cast<Cappuccino::UIBar*>(_uiComponents[4]);
	ammoBar->_transform._scaleMat[0].x = ((float)_ammo/ (float)_maxAmmo) * 500.0f;

	update(dt);
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

}


void HUD::toggleHud(bool yn)
{
	for (auto x : _uiComponents)
		x->setVisible(yn);
}