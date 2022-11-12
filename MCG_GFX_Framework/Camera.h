#pragma once
#include <GLM/glm.hpp>
#include <GLM/ext.hpp>

using namespace glm;

class Ray;

class Camera
{
public:
	Camera();
	Ray getRay(glm::vec2 pixel);
	static glm::vec3 getPos() { return m_position; };
private:
	static glm::vec3 m_position;
	mat4 view = mat4(1.0f);
	mat4 projection = mat4(1.0f);
};