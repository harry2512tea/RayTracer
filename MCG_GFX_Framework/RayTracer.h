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
	Ray();
	Ray(vec3 _origin, vec3 _direction);
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
	static glm::vec3 getColour(Ray _ray, std::list<Shared<Sphere>> *Objs, int depth, int _ignore, rayCastHit &out);
private:
	static vec3 LightPos;
	static vec3 getPointOnLine(Ray _ray, glm::vec3 _point);
	static rayCastHit RaySphereIntersect(Ray _ray, vec3 _pos, float _r);
	static vec3 getNormal(glm::vec3 _SphereCenter, vec3 _intersectPoint);
};