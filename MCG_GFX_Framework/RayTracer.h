#pragma once
#include <GLM/glm.hpp>
using namespace glm;

class rayCastHit
{
public:
	bool hit;
	float distance;
	vec3 point;
	vec3 normal;
};

class Ray
{
public:
	Ray(vec3 _origin, vec3 _direction);

	vec3 getOrigin() { return m_origin; };
	vec3 getDirection() { return m_direction; };
private:
	glm::vec3 m_origin = glm::vec3(0.0f);
	glm::vec3 m_direction = glm::vec3(0.0f);
};

class Tracer
{
public:
	glm::vec3 getColour(Ray _ray);
};