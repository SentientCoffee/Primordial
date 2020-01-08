#include "Shop.h"
#include "Cappuccino/SoundSystem.h"
#include "Cappuccino/Camera.h"

std::vector<UIInteractive*> UIInteractive::_all = {};
unsigned UIInteractive::group = 0;
UIInteractive::UIInteractive(const std::string& text, const glm::vec2& windowSize, const glm::vec2& defaultPosition, const glm::vec3& defaultColour, float defaultSize, const Cappuccino::HitBox& textBox, const std::vector<std::string>& tags)
	:Cappuccino::UIText(text, windowSize, defaultPosition, defaultColour, defaultSize), _textBox(new EmptyBox({}, textBox))
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
	sound = Cappuccino::SoundSystem::load2DSound(path);
}

void UIInteractive::playClickSound()
{
	//check if the sound exists
	if (sound >= 1000)
		return;
	Cappuccino::SoundSystem::playSound2D(sound, group, Cappuccino::SoundSystem::ChannelType::SoundEffect);
}

Empty::Empty(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes)
	:Cappuccino::GameObject(SHADER, textures, meshes)
{
	_rigidBody.setGrav(false);
}

void Empty::childUpdate(float dt)
{
}

EmptyBox::EmptyBox(const Cappuccino::Shader& SHADER, const Cappuccino::HitBox& hitbox)
	:Empty(SHADER, {}, {})
{
	_rigidBody._hitBoxes.push_back(hitbox);
}

void EmptyBox::childUpdate(float dt)
{
}

bool ShopTerminal::_cursorLocked = false;
ShopTerminal::ShopTerminal(const Cappuccino::Shader& SHADER, const std::vector<Cappuccino::Texture*>& textures, const std::vector<Cappuccino::Mesh*>& meshes, Class* player, Cappuccino::HitBox& cursorBox)
	:Cappuccino::GameObject(SHADER, textures, meshes), _triggerVolume(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(20.0f, 20.0f, 20.0f))
{
	_player = player;
	_cursorBoxPtr = &cursorBox;

	_rigidBody._moveable = true;
	_rigidBody.setGrav(false);


	//set up the ui elements of the shop
	_shopUI._uiComponents.push_back(new UIInteractive("Test",
		glm::vec2(1600.0f, 1000.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), 1.0f,
		Cappuccino::HitBox(glm::vec3(50.0f, 0.0f, 0.0f), glm::vec3(150.0f, 50.0f, 0.0f)),
		{ "dragable" }));

	for (auto x : _shopUI._uiComponents)
		x->setVisible(false);


	//set up the text prompt for when the player hits the trigger volume
	_shopPrompt._uiComponents.push_back(new Cappuccino::UIText(
		"Press E to Open Shop",
		glm::vec2(1600.0f, 1000.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		1.0f));
	_shopPrompt._uiComponents.back()->setVisible(false);

	//set up the billboard shader for the background of the shop
	_billboardShader.use();
	_billboardShader.loadOrthoProjectionMatrix(4.0f, 3.0f);
	Cappuccino::Camera camera;
	camera.lookAt(glm::vec3(0.0f, 0.0f, -3.0f));
	_billboardShader.loadViewMatrix(camera);
	_billboardShader.setUniform("image", 0);
	_shopBackground = new Billboard(&_billboardShader, {});
	_shopBackground->_transform.scale(glm::vec3(1.5f, 1.0f, 1.0f), 4.0f);

}

void ShopTerminal::childUpdate(float dt)
{
	if (_player->checkCollision(_triggerVolume, _rigidBody._position) && !_shopOpen) {

		_shopPrompt._uiComponents.back()->setVisible(true);
		
		if (_player->_input.keyboard->keyPressed(Events::E)) {
			_shopPrompt._uiComponents.back()->setVisible(false);
			_shopOpen = true;
			for (unsigned i = 0; i < _shopUI._uiComponents.size(); i++)
				_shopUI._uiComponents[i]->setVisible(true);
			_shopBackground->setActive(true);
		
		
		}
	}
	else if (!_player->checkCollision(_triggerVolume, _rigidBody._position) && _shopOpen) {
		//_shopOpen = false;
		//for (unsigned i = 0; i < _shopUI._uiComponents.size(); i++)
		//	_shopUI._uiComponents[i]->setVisible(false);
		//_shopBackground->setActive(false);
	}
	else
		_shopPrompt._uiComponents.back()->setVisible(false);
	if (!_shopOpen) {
		glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	}
	else {
		glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	//	for (unsigned i = 0; i < _shopUI._uiComponents.size(); i++) {
	//		auto element = static_cast<UIInteractive*>(_shopUI._uiComponents[i]);
	//		if (_cursorBoxPtr->checkCollision(element->getTextBox(), element->getTextBox()._position, _cursorBoxPtr->_position)) {
	//			element->setTextColour(glm::vec3(1.0f, 0.0f, 0.0f));
	//	
	//			if (_player->_input.clickListener.leftClicked()) {
	//				for (unsigned j = 0; j < element->_tags.size(); i++) {
	//	
	//					if (element->_tags[j] == "dragable") {
	//						element->setTextPosition(2.0f * glm::vec2(_cursorBoxPtr->_position.x, -_cursorBoxPtr->_position.y));
	//						element->getTextBox()._position = glm::vec3(_cursorBoxPtr->_position.x, _cursorBoxPtr->_position.y, 0.0f);
	//					}
	//	
	//				}
	//			}
	//		}
	//	
	//	
	//		auto pos = static_cast<Cappuccino::UIText*>(_shopUI._uiComponents[i])->getPosition();
	//	
	//	//	printf("x: %f,y: %f\n", pos.x, pos.y);
	//	}

	}


	_cursorLocked = _shopOpen;
	_shopPrompt.update(dt);
	_shopUI.update(dt);
}
