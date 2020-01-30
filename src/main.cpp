#include "Cappuccino/Application.h"

#include "GameplayScene711.h"
#include "MenuScene.h"
#include "Cappuccino/SoundSystem.h"
#include "Cappuccino/FrameBuffer.h"

using Application = Cappuccino::Application;
using SoundSystem = Cappuccino::SoundSystem;
using FontManager = Cappuccino::FontManager;
using Shader = Cappuccino::Shader;
using Texture = Cappuccino::Texture;
using Mesh = Cappuccino::Mesh;


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

	if (!Application::isInstantiated()) {
		unsigned border = 4;

		Cappuccino::Viewport view{ glm::vec4(0.0f,0.0f,0.0f,1.0f),glm::vec4(0.0f,0.0f,SCR_WIDTH,SCR_HEIGHT),[]() {}, };

		Application* application = new Application(SCR_WIDTH, SCR_HEIGHT, SCR_TITLE, { view });
		application->init();

		application->_clearColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		//test post proccessing effect
		char* frag = R"(#version 420 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D bloom;


uniform float greyscalePercentage = 1;

void main()
{
    vec3 col = vec3(texture(screenTexture, TexCoords.st));
    
	float brightness = dot(col.rgb, vec3(0.2126, 0.7152, 0.0722));
   // if(brightness < 1.0)
   //     col = vec3(0.0f,0.0f,0.0f);

	float average = 0.2126 * col.r + 0.7152 * col.g + 0.0722 * col.b;
	vec3 grey = vec3(average,average,average).xyz;
	vec3 finalColour = mix(grey,col,greyscalePercentage);
    FragColor = vec4(finalColour, 1.0);
})";

		Cappuccino::Framebuffer test(glm::vec2(1600.0f, 1000.0f), 1,
			[]()
		{
			CAPP_GL_CALL(glEnable(GL_DEPTH_TEST));
			CAPP_GL_CALL(glEnable(GL_CULL_FACE));
			CAPP_GL_CALL(glEnable(GL_BLEND));
			CAPP_GL_CALL(glEnable(GL_SCISSOR_TEST));
			CAPP_GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		}, std::nullopt, frag);

		Cappuccino::Framebuffer hdr(glm::vec2(1600.0f, 1000.0f), 1,
			[]()
		{
			CAPP_GL_CALL(glEnable(GL_DEPTH_TEST));
			CAPP_GL_CALL(glEnable(GL_CULL_FACE));
			CAPP_GL_CALL(glEnable(GL_BLEND));
			CAPP_GL_CALL(glEnable(GL_SCISSOR_TEST));
			CAPP_GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		});


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
