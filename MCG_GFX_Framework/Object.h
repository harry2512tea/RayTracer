#pragma once

#include <GLM/glm.hpp>
using namespace glm;

class Ray;

class Sphere
{
public:
	glm::vec3 shadePixel(Ray _ray, glm::vec3 _intersect);
	vec3 getPos() { return m_position; };
	float getRadius() { return m_radius; };

private:

	float m_radius;
	glm::vec3 m_position;

};