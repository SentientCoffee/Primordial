#include "UIPointLight.h"

UIPointLight::UIPointLight(const glm::vec2& windowSize, const std::vector<glm::vec3>& positions, const glm::vec3& ambientColour, const glm::vec3& diffuseColour, const glm::vec3& specularColour, float shininess)
	:Cappuccino::PointLight(windowSize, positions, ambientColour, diffuseColour, specularColour, shininess)
{
	Cappuccino::Shader temp("pointLightUI.vert", "pointLight.frag");

	_pointLightShader = temp;
	_pointLightShader.createShader();

	_pointLightShader.use();
	_pointLightShader.loadProjectionMatrix(windowSize.x, windowSize.y);
	_pointLightShader.setUniform("material.diffuse", 0);
	_pointLightShader.setUniform("material.specular", 1);
	_pointLightShader.setUniform("material.normalMap", 2);
	_pointLightShader.setUniform("material.emissionMap", 3);
	_pointLightShader.setUniform("material.heightMap", 4);

	_pointLightShader.setUniform("numLights", (int)positions.size());

	for (unsigned i = 0; i < _positions.size(); i++) {

		setPosition(_positions[i], i);
		setAmbient(ambientColour, i);
		setDiffuse(diffuseColour, i);
		setSpecular(specularColour, i);
		setShininess(shininess);
	}
}
