#pragma once
#include <GLM/glm.hpp>

using namespace glm;

class Ray;

class Camera
{
public:
	Ray getRay(glm::vec2 pixel);
	glm::vec3 getPos() { return m_position; };
private:
	glm::vec3 m_position;
};