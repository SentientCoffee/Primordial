#include "Shop.h"
#include "Cappuccino/SoundSystem.h"
#include "Cappuccino/Camera.h"
#include "Cappuccino/CappMath.h"
#include <Cappuccino/ResourceManager.h>
#include <Cappuccino/UI.h>

// ----------------------------------------------------------------------
// ----- UI INTERACTIVE -------------------------------------------------
// ----------------------------------------------------------------------

std::vector<UIInteractive*> UIInteractive::_all;
unsigned UIInteractive::group = 0;
UIInteractive::UIInteractive(const std::string& text, const glm::vec2& windowSize, const glm::vec2& defaultPosition, const glm::vec3& defaultColour, float defaultSize, const Cappuccino::HitBox& textBox, const std::vector<std::string>& tags)
	: UIText(text, windowSize, defaultPosition, defaultColour, defaultSize), _textBox(new EmptyBox({}, textBox))
{
	_textBox->setVisible(false);
	_all.push_back(this);
	setVisible(false);

	if (!group)
		group = Cappuccino::SoundSystem::createChannelGroup("main");

	_tags = tags;
}

void UIInteractive::updateComponent(float dt)
{
	if (isVisible())
		_textBox->setActive(true);
	else
		_textBox->setActive(false);
}

void UIInteractive::setClickSound(const std::string& path)
{
	_sound = Cappuccino::Sound(path, "UIGroup");
}

void UIInteractive::playClickSound()
{
	_sound.play();
}

// ----------------------------------------------------------------------
// ----- EMPTY/ EMPTY BOX -----------------------------------------------
// ----------------------------------------------------------------------

Empty::Empty(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	: GameObject(SHADER, textures, meshes)
{
	_rigidBody.setGrav(false);
}

void Empty::childUpdate(float dt)
{
}

EmptyBox::EmptyBox(const Cappuccino::Shader& SHADER, const Cappuccino::HitBox& hitbox)
	: Empty(SHADER, {}, {})
{
	_rigidBody._hitBoxes.push_back(hitbox);
}

void EmptyBox::childUpdate(float dt)
{
}

// ----------------------------------------------------------------------
// ----- SHOP TERMINAL --------------------------------------------------
// ----------------------------------------------------------------------

bool ShopTerminal::cursorLocked = false;
bool ShopTerminal::damageBought = false;
bool ShopTerminal::ammoBought = false;

std::vector<Cappuccino::Sound> ShopTerminal::_greeting;
std::vector<Cappuccino::Sound> ShopTerminal::_success;
std::vector<Cappuccino::Sound> ShopTerminal::_declined;
std::vector<Cappuccino::Sound> ShopTerminal::_farewell;
std::vector<Cappuccino::Sound> ShopTerminal::_pain;

ShopTerminal::ShopTerminal(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, Class* player, Cappuccino::HitBox& cursorBox) :
	GameObject(SHADER, textures, meshes), _triggerVolume(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(20.0f, 20.0f, 20.0f))
{
	const std::string _ = "Shop/";

	//im so lazy
	static bool first = true;
	if (first) {
		//push back all the sounds, only once ever
		for (unsigned i = 0; i < 5; i++) {
			_greeting.emplace_back(_ + "18 greeting " + std::to_string(i + 1) + ".wav", "UIGroup");
		}
		for (unsigned i = 0; i < 7; i++) {
			_success.emplace_back(_ + "19 success " + std::to_string(i + 1) + ".wav", "UIGroup");
		}
		for (unsigned i = 0; i < 11; i++) {
			_declined.emplace_back(_ + "20 declined " + std::to_string(i + 1) + ".wav", "UIGroup");
		}
		for (unsigned i = 0; i < 6; i++) {
			_farewell.emplace_back(_ + "21 farewell " + std::to_string(i + 1) + ".wav", "UIGroup");
		}
		for (unsigned i = 0; i < 6; i++) {
			_pain.emplace_back(_ + "22 pain " + std::to_string(i + 1) + ".wav", "UIGroup");
		}
	}
	_player = player;
	_cursorBoxPtr = &cursorBox;

	_rigidBody._moveable = true;
	_rigidBody.setGrav(false);

	_billboardShader = *Cappuccino::ShaderLibrary::loadShader("Billboard", "billboardShader.vert", "billboardShader.frag");

	// Set up the ui elements of the shop
	_shopUI._uiComponents.reserve(10);

	// ----------------------------
	// -- Currency count ----------
	// ----------------------------

	_shopUI._uiComponents.push_back(new UIInteractive("SEDNIUM",
		glm::vec2(1600.0f, 1000.0f),
		glm::vec2(390.0f, 520.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		1.0f,
		Cappuccino::HitBox(glm::vec3(5000.0f, 200.0f, 0.0f), glm::vec3(15000.0f, 250.0f, 0.0f)),
		{}
	));

	// ----------------------------
	// -- HP price ----------------
	// ----------------------------
	
	_shopUI._uiComponents.push_back(new UIInteractive("+20% HEALTH PRICE",
		glm::vec2(1600.0f, 1000.0f),
		glm::vec2(-870.0f, 180.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), 1.0f,
		Cappuccino::HitBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(350.0f, 50.0f, 0.0f)),
		{ "$", "HP" }
	));
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->setClickSound("uiClick.wav");
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->setPrice(300);
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->getTextBox()._position = glm::vec3(-816.0f / 2.0f + 100.0f, -232.0f / 2.0f, 0.0f);

	// ----------------------------
	// -- Ammo price --------------
	// ----------------------------
	
	_shopUI._uiComponents.push_back(new UIInteractive("+20% AMMO PRICE",
		glm::vec2(1600.0f, 1000.0f),
		glm::vec2(-870.0f, -200.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), 1.0f,
		Cappuccino::HitBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(450.0f, 50.0f, 0.0f)),
		{ "$", "AMMO" }
	));
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->setClickSound("uiClick.wav");
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->setPrice(300);
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->getTextBox()._position = glm::vec3(-816.0f / 2 + 100.0f, 180.0f / 2, 0.0f);

	// -----------------------------
	// -- Damage boost price -------
	// -----------------------------

	_shopUI._uiComponents.push_back(new UIInteractive("+10% DAMAGE BOOST PRICE",
		glm::vec2(1600.0f, 1000.0f),
		glm::vec2(230.0f, 180.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), 1.0f,
		Cappuccino::HitBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(450.0f, 50.0f, 0.0f)),
		{ "$", "UPDMG" }
	));
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->setClickSound("uiClick.wav");
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->setPrice(550);
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->getTextBox()._position = glm::vec3(350.0f / 2, -226.0f / 2, 0.0f);

	// ----------------------------
	// -- Clip size price ---------
	// ----------------------------
	
	_shopUI._uiComponents.push_back(new UIInteractive("+15% AMMO CAPACITY PRICE",
		glm::vec2(1600.0f, 1000.0f),
		glm::vec2(230.0f, -200.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), 1.0f,
		Cappuccino::HitBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(450.0f, 50.0f, 0.0f)),
		{ "$", "UPAMM" }
	));
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->setClickSound("uiClick.wav");
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->setPrice(550);
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->getTextBox()._position = glm::vec3(358.0f / 2, 178.0f / 2, 0.0f);

	// ----------------------------
	// -- HP label ----------------
	// ----------------------------
	
	_shopUI._uiComponents.push_back(new UIInteractive("+20% HEALTH",
		glm::vec2(1600.0f, 1000.0f),
		glm::vec2(-870.0f, 280.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), 1.25f,
		Cappuccino::HitBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(350.0f, 50.0f, 0.0f)),
		{ "$", "HP" }
	));
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->setClickSound("uiClick.wav");
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->setPrice(300);
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->getTextBox()._position = glm::vec3(-816.0f / 2.0f + 100.0f, -232.0f / 2.0f, 0.0f);

	// ----------------------------
	// -- Ammo label --------------
	// ----------------------------
	
	_shopUI._uiComponents.push_back(new UIInteractive("+20% AMMO",
		glm::vec2(1600.0f, 1000.0f),
		glm::vec2(-870.0f, -100.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), 1.25f,
		Cappuccino::HitBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(450.0f, 50.0f, 0.0f)),
		{ "$", "AMMO" }
	));
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->setClickSound("uiClick.wav");
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->setPrice(300);
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->getTextBox()._position = glm::vec3(-816.0f / 2 + 100.0f, 180.0f / 2, 0.0f);

	// -----------------------------
	// -- Damage boost label -------
	// -----------------------------

	_shopUI._uiComponents.push_back(new UIInteractive("+10% DAMAGE BOOST",
		glm::vec2(1600.0f, 1000.0f),
		glm::vec2(230.0f, 280.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), 1.25f,
		Cappuccino::HitBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(450.0f, 50.0f, 0.0f)),
		{ "$", "UPDMG" }
	));
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->setClickSound("uiClick.wav");
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->setPrice(550);
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->getTextBox()._position = glm::vec3(350.0f / 2, -226.0f / 2, 0.0f);

	// ----------------------------
	// -- Ammo cap label ----------
	// ----------------------------
	
	_shopUI._uiComponents.push_back(new UIInteractive("+15% AMMO CAPACITY",
		glm::vec2(1600.0f, 1000.0f),
		glm::vec2(230.0f, -100.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), 1.25f,
		Cappuccino::HitBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(450.0f, 50.0f, 0.0f)),
		{ "$", "UPAMM" }
	));
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->setClickSound("uiClick.wav");
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->setPrice(550);
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->getTextBox()._position = glm::vec3(358.0f / 2, 178.0f / 2, 0.0f);

	// ----------------------------
	// -- Exit --------------------
	// ----------------------------
	
	_shopUI._uiComponents.push_back(new UIInteractive("Exit",
		glm::vec2(1600.0f, 1000.0f),
		glm::vec2(765.0f, -570.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), 2.0f,
		Cappuccino::HitBox(glm::vec3(820.0f / 2 + 50.f, 570.0f / 2, 0.0f), glm::vec3(820.0f / 2 + 150.0f, 50.0f + 570.0f / 2, 0.0f)),
		{ "Exit" }
	));
	static_cast<UIInteractive*>(_shopUI._uiComponents.back())->setClickSound("uiClick.wav");

	for (auto x : _shopUI._uiComponents)
		x->setVisible(false);


	//set up the text prompt for when the player hits the trigger volume
	_shopPrompt._uiComponents.push_back(new Cappuccino::UIText("Press E to Open Shop",
		glm::vec2(1600.0f, 1000.0f),
		glm::vec2(-270.0f, -200.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		1.0f
	));
	_shopPrompt._uiComponents.back()->setVisible(false);

	//set up the billboard shader for the background of the shop
	_billboardShader.use();
	_billboardShader.loadOrthoProjectionMatrix(4.0f, 3.0f);
	Cappuccino::Camera camera;
	camera.lookAt(glm::vec3(0.0f, 0.0f, -3.0f));
	_billboardShader.loadViewMatrix(camera);
	_billboardShader.setUniform("image", 0);
	_shopBackground = new Billboard(&_billboardShader, { Cappuccino::TextureLibrary::loadTexture("Shop background", "shop.png",Cappuccino::TextureType::PBRAlbedo) });

	_finalTransform = _shopBackground->_transform;
	_finalTransform.scale(glm::vec3(1.5f, 1.0f, 1.0f), 4.0f);



}

void ShopTerminal::childUpdate(float dt)
{
	static bool exit = false;
	static bool first = true;

	static auto originalScale = _shopBackground->_transform._scaleMat;
	static float u = 0.0f;
	static bool shopHudOff = false;


	//check if the player is in range
	if (_player->checkCollision(_triggerVolume, _rigidBody._position) && !_shopOpen) {

		//make the shop prompt visible
		_shopPrompt._uiComponents.back()->setVisible(true);

		//open the shop if the player presses E
		if (_player->_input.keyboard->keyPressed(Cappuccino::KeyEvent::E)) {
			_shopPrompt._uiComponents.back()->setVisible(false);
			_shopOpen = true;
			_shopBackground->setActive(true);
			const int index = Cappuccino::randomInt(0, (int)_greeting.size() - 1);
			_greeting[index].play();
		}
	}
	else if (!_player->checkCollision(_triggerVolume, _rigidBody._position) && _shopOpen) {
		//exit if the player is out of range of the shop and the shop is open
		//exit = true;
	}
	else {
		//in any other case, just make sure that the prompt isn't visible
		_shopPrompt._uiComponents.back()->setVisible(false);
	}

	static bool forceOpen = false;
	if (!_shopOpen) {
		/// Remove this after testing
		#if 0
		static bool done = false;
		if (_player->_input.keyboard->keyPressed(Cappuccino::KeyEvent::N) && !done) {
			forceOpen ^= 1;
			done = true;
			_shopPrompt._uiComponents.back()->setVisible(false);
			_shopOpen = true;
			_shopBackground->setActive(true);
		}
		else if (_player->_input.keyboard->keyReleased(Cappuccino::KeyEvent::N))
			done = false;
		#endif
		
		_player->setShopping(false);
		if (forceOpen)
			glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else {
		_player->setShopping(true);
		//unlock the cursor if we're in the shop

		//does the math to show that coordinates are being converted back to their original frame
		if (_cursorBoxPtr->_position.x <= -595.0f)
			glfwSetCursorPos(glfwGetCurrentContext(), 800.0 - 595.0, _cursorBoxPtr->_position.y + 500.0);
		else if (_cursorBoxPtr->_position.x >= 595.0f)
			glfwSetCursorPos(glfwGetCurrentContext(), 800.0 + 595.0, _cursorBoxPtr->_position.y + 500.0);
		if (_cursorBoxPtr->_position.y <= -290.0f)
			glfwSetCursorPos(glfwGetCurrentContext(), _cursorBoxPtr->_position.x + 800.0, 500.0 - 290.0);
		else if (_cursorBoxPtr->_position.y >= 290.0f)
			glfwSetCursorPos(glfwGetCurrentContext(), _cursorBoxPtr->_position.x + 800.0, 500.0 + 290.0);

		//set the text element to display the proper currency
		auto currency = static_cast<UIInteractive*>(_shopUI._uiComponents[0]);
		auto health   = static_cast<UIInteractive*>(_shopUI._uiComponents[1]);
		auto ammo     = static_cast<UIInteractive*>(_shopUI._uiComponents[2]);
		auto dmgBoost = static_cast<UIInteractive*>(_shopUI._uiComponents[3]);
		auto ammoCap  = static_cast<UIInteractive*>(_shopUI._uiComponents[4]);

		currency->setText("Sednium: " + std::to_string(_player->getCurrency()));
		health->setText("Cost: " + std::to_string(health->getPrice()));
		ammo->setText("Cost: " + std::to_string(ammo->getPrice()));

		if(!damageBought) {
			dmgBoost->setText("Cost: " + std::to_string(dmgBoost->getPrice()));
		}
		else {
			dmgBoost->setText("Sold out");
		}

		if(!ammoBought) {
			ammoCap->setText("Cost: " + std::to_string(ammoCap->getPrice()));
		}
		else {
			ammoCap->setText("Sold out");
		}
		
		//lerp the scale
		if (!(u >= 1.0f) && !exit) {
			u += dt * 5.0f;

			_shopBackground->_transform._scaleMat = Cappuccino::Math::lerp(originalScale, _finalTransform._scaleMat, u);
			if (u >= 1.0f)
				u = 1.0f;
		}

		if (u >= 1.0f && !exit) {

			if (first) {
				for(auto uiComponent : _shopUI._uiComponents)
					uiComponent->setVisible(true);

				//_player->toggleHud();

				// TODO: REMOVE AFTER TESTING
				// Please for the LOVE of CHRIST remove this already
				///REMOVE AFTER TESTING
				{
					for (unsigned i = 0; i < 5000; i++)
						_player->addCurrency();
				}
				///REMOVE AFTER TESTING
				
				_player->setCanShoot(false);

				first = false;
				glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				glfwSetCursorPos(glfwGetCurrentContext(), 800.0f, 500.0f);

			}

			//if the player presses escape, exit the shop
			if (_player->_input.keyboard->keyPressed(Cappuccino::KeyEvent::ESCAPE))
				exit = true;

			// temporary code for moving around the text so that i can decide where the text should go
			// Much indent
			for(auto uiComponent : _shopUI._uiComponents) {
				if (exit) {
					break;
				}
				auto element = dynamic_cast<UIInteractive*>(uiComponent);
				if (_cursorBoxPtr->checkCollision(element->getTextBox(), element->getTextBox()._position, _cursorBoxPtr->_position)) {
					element->setTextColour(glm::vec3(1.0f, 0.0f, 0.0f));

					static bool hasClicked = false;
					if (_player->_input.clickListener.leftClicked()) {
						//buy the item, make the appropriate sound
						if (!hasClicked) {

							for (unsigned j = 0; j < element->_tags.size(); j++) {
								if (element->_tags[j] == "$") {
									static bool correct = false;//correct is true if the player buys an item
									if (_player->getCurrency() - (int)element->getPrice() > 0) {
										for (const auto& x : element->_tags) {
											if (x == "HP") {
												if (_player->getMaxHp() != _player->getHealth())
												{
													const float newHp = _player->getHealth() + _player->getMaxHp() * 0.2f;
													_player->setHealth(newHp >= _player->getMaxHp() ? _player->getMaxHp() : newHp);
													_player->getCurrency() -= (int)element->getPrice();
												}
												else
												{
													//output some sorta sound or message
												}
											}
											else if (x == "AMMO") {
												if (_player->getPrimary()->getMaxAmmo() != _player->getPrimary()->getAmmoCount())
												{
													_player->addAmmo();
													_player->getCurrency() -= (int)element->getPrice();
												}
												else
												{
													//output some sorta sound or message
												}
											}
											else if (x == "UPDMG") {
												if (!damageBought) {
													_player->getGun()->setDamage(_player->getGun()->getDamage() * 1.1f);
													_player->getCurrency() -= (int)element->getPrice();
												}
												damageBought = true;

											}
											else if (x == "UPAMM") {
												static unsigned numTimesAmmoBought = 0;
												if (!ammoBought) {
													_player->getGun()->setMaxAmmo((int)((float)_player->getGun()->getMaxAmmo() * 1.15f));
													_player->getCurrency() -= (int)element->getPrice();
													++numTimesAmmoBought;
												}
												if(numTimesAmmoBought >= 2) {
													ammoBought = true;
													numTimesAmmoBought = 0;
												}

											}

										}

										element->setPrice((int)((float)element->getPrice() * 1.2f));
										const int index = Cappuccino::randomInt(0, (int)_success.size() - 1);
										_success[index].play();
										correct = true;
									}
									else {
										static unsigned index = 0;
										if (correct) {
											index = 0;
										}
										_declined[index].play();
										correct = false;
										index++;
										if (index > _declined.size() - 1) {
											index = 0;
										}
									}
									//printf("%d %d\n", _player->getCurrency(),element->getPrice());
									break;
								}
								if (element->_tags[j] == "Exit") {
									exit = true;
									break;
								}
							}

							// if the player clicked the exit button, exit the shop
							element->playClickSound();
							hasClicked = true;
						}

						// TODO: REMOVE AFTER TESTING
						///REMOVE AFTER TESTING, USED TO DRAG UI ELEMENTS INTO PLACE
						for (unsigned j = 0; j < element->_tags.size(); j++) {

							if (element->_tags[j] == "draggable") {
								element->setTextPosition(2.0f * glm::vec2(_cursorBoxPtr->_position.x, -_cursorBoxPtr->_position.y));
								element->getTextBox()._position = glm::vec3(_cursorBoxPtr->_position.x, _cursorBoxPtr->_position.y, 0.0f);
								printf("$300: %f, %f\n", element->getPosition().x, element->getPosition().y);
							}

						}
					}
					else
						hasClicked = false;
				}
				else {
					for(const auto& tag: element->_tags) {
						if(tag == "UPDMG" && damageBought || tag == "UPAMM" && ammoBought) {
							element->setTextColour(glm::vec3(0.4f, 0.4f, 0.4f));
						}
						else {
							element->setTextColour(glm::vec3(1.0f, 1.0f, 1.0f));
						}
					}
				}
				//auto pos = static_cast<Cappuccino::UIText*>(_shopUI._uiComponents[i])->getPosition();
				//printf("%s:\nx: %f\ty: %f\n\n", element->getText().c_str(), pos.x, pos.y);
			}

		}
	}

	if (exit) {
		if (!shopHudOff) {
			u = 0.0f;
			for(auto uiComponent : _shopUI._uiComponents) {
				uiComponent->setVisible(false);
			}
			shopHudOff = true;
		}
	}

	if (!(u >= 1.0f) && exit) {
		u += dt * 5.0f;

		_shopBackground->_transform._scaleMat = Cappuccino::Math::lerp(_finalTransform._scaleMat, originalScale, u);
		if (u >= 1.0f)
			u = 1.0f;
	}

	//exit the shop when exit is true
	if (u >= 1.0f && exit) {

		_shopBackground->setActive(false);

		_shopOpen = false;
		_player->setCanShoot(true);
		first = true;
		exit = false;
		shopHudOff = false;
		u = 0.0f;
		const int index = Cappuccino::randomInt(0, (int)_farewell.size() - 1);
		_farewell[index].play();
	}


	cursorLocked = _shopOpen;
	_shopPrompt.update(dt);
	_shopUI.update(dt);
}
