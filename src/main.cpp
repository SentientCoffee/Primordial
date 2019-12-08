#include "Cappuccino/Application.h"

#include "GameplayScene711.h"
#include "MenuScene.h"
#include "Cappuccino/SoundSystem.h"

using Application = Cappuccino::Application;
using SoundSystem = Cappuccino::SoundSystem;
using FontManager = Cappuccino::FontManager;
using Shader      = Cappuccino::Shader;
using Texture     = Cappuccino::Texture;
using Mesh        = Cappuccino::Mesh;


#pragma region PROGRAM SETTINGS

constexpr GLuint  SCR_WIDTH = 1600;
constexpr GLuint  SCR_HEIGHT = 1000;
constexpr GLchar* SCR_TITLE = "Primordial Alpha 11/19";

#pragma endregion

// This piece of code was given to us by Shawn Matthews
// Auto-magically changes OpenGL to use the high performance GPU to render rather than the iGPU
// (for dual GPU systems)
extern "C" {
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x01;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 0x01;
}

int main() {

	if(!Application::isInstantiated()) {
		unsigned border = 4;
		Cappuccino::Viewport viewport{ glm::vec4(0.5f,0.5f,0.5f,1.0f),glm::vec4(0.0f + border,0.0f + border,(SCR_WIDTH / 2.0f) - 2 * border,(SCR_HEIGHT / 2.0f) - 2 * border),[]() {CAPP_GL_CALL(glDisable(GL_CULL_FACE)); },GL_LINE };
		Cappuccino::Viewport viewport2{ glm::vec4(0.5f,0.5f,0.5f,1.0f),glm::vec4(border + SCR_WIDTH / 2.0f,border + 0.0f,(SCR_WIDTH / 2.0f) - 2 * border,(SCR_HEIGHT / 2.0f) - 2 * border),[]() {CAPP_GL_CALL(glEnable(GL_CULL_FACE)); } };
		Cappuccino::Viewport viewport3{ glm::vec4(0.5f,0.5f,0.5f,1.0f),glm::vec4(border + SCR_WIDTH / 2.0f,border + SCR_HEIGHT / 2.0f,(SCR_WIDTH / 2.0f) - 2 * border,(SCR_HEIGHT / 2.0f) - 2 * border),[]() {} };
		Cappuccino::Viewport viewport4{ glm::vec4(0.5f,0.5f,0.5f,1.0f),glm::vec4(border + 0.0f,border + SCR_HEIGHT / 2.0f,(SCR_WIDTH / 2.0f) - 2 * border,(SCR_HEIGHT / 2.0f) - 2 * border),[]() {} };

		Cappuccino::Viewport view{ glm::vec4(0.0f,0.0f,0.0f,1.0f),glm::vec4(0.0f,0.0f,SCR_WIDTH,SCR_HEIGHT),[]() {}, };

		Application* application = new Application(SCR_WIDTH, SCR_HEIGHT, SCR_TITLE, { view });
		application->init();

		application->_clearColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		SoundSystem::setDefaultPath("./Assets/Sounds/");
		FontManager::setDefaultPath("./Assets/Fonts/");
		Shader::setDefaultPath("./Assets/Shaders/");
		Mesh::setDefaultPath("./Assets/Meshes/");
		Texture::setDefaultPath("./Assets/Textures/");

		FontManager::loadTypeFace("Viper Nora.ttf");

		MenuScene* m = new MenuScene(true);
		m->init();

		GameplayScene* g = new GameplayScene(false);
		

		application->run();
		delete application;
	}

	return 0;

}
