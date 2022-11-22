#pragma once
#include <GLM/glm.hpp>
#include <GLM/ext.hpp>
#include <GLM/fwd.hpp>
#include <memory>
#include <list>

using namespace glm;


#define Shared std::shared_ptr

class Ray;

class rayCastHit;

class Sphere;

class Plane
{
public:
	Plane(vec3 _position = vec3(0.0f, 3.0f, 0.0f), vec3 _normal = vec3(0.0f, -1.0f, 0.0f), vec3 _colour = vec3(1.0f));

	rayCastHit findIntersection(Ray _ray);

	vec3 ShadePixel(Ray _ray, glm::vec3 _intersect, vec3 _LightPos, int depth, std::list<Shared<Sphere>>* Objs, std::list<Shared<Plane>>* Planes);

private:
	int ID;

	vec3 m_position;
	vec3 m_normal;

	vec3 albedo = vec3(1.0f, 1.0f, 1.0f);

	vec3 F0 = vec3(0.05f);
	float Roughness = 0.5f;
	int maxDepth = 1;

	vec3 DiffuseBRDF(vec3 _intersect, vec3 _lightPos);
	vec3 SpecularBRDF(vec3 _intersect, vec3 _lightPos);
	vec3 fresnelSchlick(float cosTheta);
	float GeomSchlickGGX(float NdotV);
	float DistributionGGX(vec3 N, vec3 H);
	float GeomSmith(vec3 N, vec3 V, vec3 L);
};