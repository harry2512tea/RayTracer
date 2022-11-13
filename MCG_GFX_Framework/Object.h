#pragma once

#include <GLM/glm.hpp>
using namespace glm;

class Ray;

class Sphere
{
public:
	glm::vec3 shadePixel(Ray _ray, glm::vec3 _intersect, vec3 _LightPos);
	vec3 getPos() { return m_position; };
	float getRadius() { return m_radius; };
	vec3 getNormal(vec3 _intersect);
private:
	vec3 colour = vec3(0.7f, 0.0f,0.7f);
	float m_radius = 1.0f;
	glm::vec3 m_position{0.0f, 0.0f, -8.0f};

};