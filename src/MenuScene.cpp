#include "MenuScene.h"
#include "Cappuccino/CappMacros.h"
#include "Options.h"

MenuScene::MenuScene(bool isActive)
	:Cappuccino::Scene(isActive), _in(true, std::nullopt), cursorBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(100.0f, 100.0f, 100.0f)), startBox(glm::vec3(0.0f, -75.0f, 0.0f), glm::vec3(200.0f, 100.0f, 200.0f)),
	commandoBox(glm::vec3(250.0f, -75.0f, 0.0f), glm::vec3(450.0f, 100.0f, 250.0f)), assaultBox(glm::vec3(-250.0f, -75.0f, 0.0f), glm::vec3(250.0f, 100.0f, 200.0f))
{

	menuShader = new Cappuccino::Shader("billboardShader.vert", "billboardShader.frag");
	logo = new Billboard(menuShader, { new Cappuccino::Texture("logo.jpg",Cappuccino::TextureType::DiffuseMap) });
	logo->_rigidBody._position = glm::vec3(0.0f, 0.0f, 3.0f);
	logo->_transform.scale(glm::vec3(1.0f, 1.0f, 1.0f), 5.0f);
	ui._uiComponents.push_back(new Cappuccino::UIText("Start", glm::vec2(1600.0f, 1200.0f), glm::vec2(-100.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.5f));
	ui._uiComponents.push_back(new Cappuccino::UIText("Commando", glm::vec2(1600.0f, 1200.0f), glm::vec2(200.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1.5f));
	ui._uiComponents.back()->setVisible(false);
	ui._uiComponents.push_back(new Cappuccino::UIText("Demolitionist", glm::vec2(1600.0f, 1200.0f), glm::vec2(-600.0f, 0.0f), glm::vec3(1.0f, 0.0f, 1.0f), 1.5f));
	ui._uiComponents.back()->setVisible(false);
	ui._uiComponents.push_back(new Cappuccino::UIText("P R I M O R D I A L", glm::vec2(1600.0f, 1200.0f), glm::vec2(-600.0f, 600.0f), glm::vec3(1.0f, 0.0f, 0.0f), 2.5f));
	menuShader->use();
	menuShader->loadOrthoProjectionMatrix(4.0f, 3.0f);
	camera.lookAt(glm::vec3(0.0f, 0.0f, -3.0f));
	menuShader->loadViewMatrix(camera);
	menuShader->setUniform("image", 0);
}

bool MenuScene::init()
{
	_initialized = true;
	_shouldExit = false;

	logo->setActive(true);

	ui._uiComponents.front()->setVisible(true);
	ui._uiComponents.back()->setVisible(true);

	return true;
}

bool MenuScene::exit()
{
	_initialized = false;
	_shouldExit = true;

	logo->setActive(false);

	for (auto x : ui._uiComponents)
		x->setVisible(false);

	return true;
}

void MenuScene::childUpdate(float dt)
{
	static bool characterSelect = false;
	cursorBox._position = glm::vec3(cursorPos.x, cursorPos.y, 0.0f);
	//CAPP_PRINT("%f %f\n",cursorBox._position.x, cursorBox._position.y);
	//CAPP_PRINT("box %f %f\n",startBox._position.x, startBox._position.y);



	static bool change = false;

	//assault
	if (cursorBox.checkCollision(commandoBox, commandoBox._position, cursorBox._position) && characterSelect) {
		dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[1])->setTextColour(glm::vec3(1.0f, 0.0f, 0.0f));
		if (_in.clickListener.leftClicked()) {
			Options::Commando = true;
			change = true;
		}
	}
	else if (!cursorBox.checkCollision(commandoBox, commandoBox._position, cursorBox._position) && characterSelect)
		dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[1])->setTextColour(glm::vec3(0.0f, 1.0f, 0.0f));

	//commando
	if (cursorBox.checkCollision(assaultBox, assaultBox._position, cursorBox._position) && characterSelect) {
		dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[2])->setTextColour(glm::vec3(1.0f, 0.0f, 0.0f));
		if (_in.clickListener.leftClicked()) {
			Options::Assault = true;
			change = true;
		}
	}
	else if (!cursorBox.checkCollision(assaultBox, assaultBox._position, cursorBox._position) && characterSelect)
		dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[2])->setTextColour(glm::vec3(1.0f, 0.0f, 1.0f));


	//start button
	if (cursorBox.checkCollision(startBox, startBox._position, cursorBox._position) && !characterSelect) {
		dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[0])->setTextColour(glm::vec3(1.0f, 0.0f, 0.0f));

		if (_in.clickListener.leftClicked()) {
			characterSelect = true;
			dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[0])->setVisible(false);
			dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[1])->setVisible(true);
			dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[2])->setVisible(true);

		}
		//Cappuccino::SceneManager::changeScene(1);
	}
	else if (!cursorBox.checkCollision(startBox, startBox._position, cursorBox._position) && !characterSelect)
		dynamic_cast<Cappuccino::UIText*>(ui._uiComponents[0])->setTextColour(glm::vec3(1.0f, 1.0f, 1.0f));


	ui.update(dt);

	if (change)
		Cappuccino::SceneManager::changeScene(1);


}

void MenuScene::mouseFunction(double xpos, double ypos)
{
	cursorPos = glm::vec2(xpos, ypos);
	cursorPos -= glm::vec2(800.0f, 600.0f);
}

void MenuScene::clickFunction(int button, int action, int mods)
{
	_in.clickListener.checkClick(button, action, mods);
}
