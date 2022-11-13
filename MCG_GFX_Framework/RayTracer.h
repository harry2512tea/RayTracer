#pragma once
#include <GLM/glm.hpp>
#include <memory>
#include <list>

#define Shared std::shared_ptr
#define Weak std::weak_ptr

using namespace glm;

class Camera;

class Sphere;

class rayCastHit
{
public:
	bool hit = false;
	float distance;
	vec3 point;
	vec3 normal;
};

class Ray
{
public:

	vec3 getOrigin() { return m_origin; };
	vec3 getDirection() { return m_direction; };
private:
	friend class Camera;
	glm::vec3 m_origin = glm::vec3(0.0f);
	glm::vec3 m_direction = glm::vec3(0.0f, 0.0f, -1.0f);
};

class Tracer
{
public:
	glm::vec3 getColour(Ray _ray, std::list<Shared<Sphere>> *Objs);
private:
	vec3 LightPos = vec3(-5.0f, -3.0f, -2.0f);
	vec3 getPointOnLine(Ray _ray, glm::vec3 _point);
	rayCastHit RaySphereIntersect(Ray _ray, vec3 _pos, float _r);
	vec3 getNormal(glm::vec3 _SphereCenter, vec3 _intersectPoint);
};