#include "RayTracer.h"
#include "Object.h"

#include <iostream>

vec3 Tracer::LightPos = vec3(0.0f, -0.0f, -5.0f);

glm::vec3 Tracer::getColour(Ray _ray, std::list<Shared<Sphere>>* Objs, int depth, int _ignore)
{
	rayCastHit hit;
	hit.distance = -1;
	hit.hit = false;
	auto finalIt = Objs->begin();
	for (auto it = Objs->begin(); it != Objs->end(); it++)
	{
		if ((*it)->ID != _ignore)
		{
			rayCastHit temp = RaySphereIntersect(_ray, (*it)->getPos(), (*it)->getRadius());
			//std::cout << temp.hit << std::endl;
			if (temp.hit)
			{
				if (temp.distance < hit.distance || hit.distance == -1)
				{
					//std::cout << temp.distance << std::endl;
					hit = temp;
					finalIt = it;
				}
			}
		}
	}
	
	if (hit.hit)
	{
		return (*finalIt)->shadePixel(_ray, hit.point, LightPos, depth, Objs);
	}
	else
	{
		return vec3(1.0f);
	}
}


rayCastHit Tracer::RaySphereIntersect(Ray _ray, vec3 _pos, float _r)
{
	rayCastHit hit;
	vec3 a = _ray.getOrigin();
	vec3 n = _ray.getDirection();
	vec3 P = _pos;
	vec3 pointOnLine = getPointOnLine(_ray, _pos);
	//float d = distance(P, pointOnLine);
	

	

	float dist = distance(P, pointOnLine);
	
	//float mag = 
	//std::cout << mag << std::endl;

	if (dist <= _r)
	{
		float d = length((P - a - (dot((P - a), n) * n)));
		float x = sqrt((_r * _r) - (d * d));
		vec3 point = a + ((dot((P - a), n) - x) * n);

		float pointMag = dot(point - _ray.getOrigin(), _ray.getDirection());
		float lineMag = dot(pointOnLine - _ray.getOrigin(), _ray.getDirection());

		//std::cout << dot(point - _ray.getOrigin(), _ray.getDirection()) << " " << dot(pointOnLine - _ray.getOrigin(), _ray.getDirection()) << std::endl;

		if (pointMag > 0)
		{
			if (pointMag < lineMag)
			{
				//hit
				hit.hit = true;
				hit.point = point;
				hit.distance = distance(point, _ray.getOrigin());
				hit.normal = getNormal(_pos, point);
			}
			else
			{
				vec3 point = a + ((dot((P - a), n) + x) * n);
				hit.hit = true;
				hit.point = point;
				hit.distance = distance(point, _ray.getOrigin());
				hit.normal = getNormal(_pos, point);
			}
		}
		else
		{
			hit.hit = false;
		}

		
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

Ray::Ray(vec3 _origin, vec3 _direction)
{
	m_origin = _origin;
	m_direction = _direction;
}
Ray::Ray()
{

};