#pragma once
#include "Cappuccino/PointLight.h"

class UIPointLight : public Cappuccino::PointLight {
public:
	UIPointLight(const glm::vec2& windowSize, const std::vector<glm::vec3>& positions, const glm::vec3& ambientColour, const glm::vec3& diffuseColour, const glm::vec3& specularColour, float shininess);

};