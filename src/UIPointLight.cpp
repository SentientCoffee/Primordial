#include "UIPointLight.h"

UIPointLight::UIPointLight(const glm::vec2& windowSize, const glm::vec3& position, const glm::vec3& ambientColour, const glm::vec3& diffuseColour, const glm::vec3& specularColour, float shininess)
	:Cappuccino::PointLight(windowSize, position, ambientColour, diffuseColour, specularColour, shininess)
{
	Cappuccino::Shader temp("pointLightUI.vert", "pointLight.frag");

	_pointLightShader = temp;
	_pointLightShader.createShader();

	_pointLightShader.use();
	_pointLightShader.loadProjectionMatrix(windowSize.x, windowSize.y);
	_pointLightShader.setUniform("material.diffuse", 0);
	_pointLightShader.setUniform("material.specular", 1);

	setPosition(position);
	setAmbient(ambientColour);
	setDiffuse(diffuseColour);
	setSpecular(specularColour);
	setShininess(shininess);
}
