#include "RayTracer.h"
#include "Object.h"

#include <iostream>

glm::vec3 Tracer::getColour(Ray _ray, std::list<Shared<Sphere>>* Objs)
{
	rayCastHit hit;
	hit.distance = -1;
	hit.hit = false;
	auto finalIt = Objs->begin();
	for (auto it = Objs->begin(); it != Objs->end(); it++)
	{
		rayCastHit temp = RaySphereIntersect(_ray, (*it)->getPos(), (*it)->getRadius());
		//std::cout << temp.hit << std::endl;
		if (temp.hit)
		{
			if (temp.distance < hit.distance || hit.distance == -1)
			{
				hit = temp;
				finalIt = it;
			}
		}
	}
	
	if (hit.hit)
	{
		return (*finalIt)->shadePixel(_ray, hit.point, LightPos);
	}
	else
	{
		return vec3(0.2f);
	}
}


rayCastHit Tracer::RaySphereIntersect(Ray _ray, vec3 _pos, float _r)
{
	rayCastHit hit;
	vec3 a = _ray.getOrigin();
	vec3 n = _ray.getDirection();
	vec3 P = _pos;
	vec3 pointOnLine = getPointOnLine(_ray, _pos);
	float d = distance(P, pointOnLine);
	float x = sqrt((_r * _r) - (d * d));

	vec3 point = a + ((dot((P - a), n) - x) * n);

	float dist = distance(P, pointOnLine);

	if (dist <= _r)
	{
		hit.hit = true;
		hit.point = point;
		hit.distance = distance(point, _ray.getOrigin());
		hit.normal = getNormal(_pos, point);
	}
	else
	{
		hit.hit = false;
	}

	return hit;
}

vec3 Tracer::getPointOnLine(Ray _ray, glm::vec3 _point)
{
	vec3 a = _ray.getOrigin();
	vec3 n = _ray.getDirection();
	vec3 P = _point;
	vec3 point = a + (dot((P - a), n) * n);
	return point;
}

vec3 Tracer::getNormal(glm::vec3 _SphereCenter, vec3 _intersectPoint)
{
	return normalize(_intersectPoint - _SphereCenter);
}