#pragma once

#include <GLM/glm.hpp>
#include <GLM/ext.hpp>
#include <GLM/fwd.hpp>
#include <memory>
#include <list>

#define Shared std::shared_ptr
#define Weak std::weak_ptr

using namespace glm;

class Ray;

class Plane;

class Sphere
{
public:
	Sphere(int _ID, vec3 Pos, vec3 albedo);
	glm::vec3 shadePixel(Ray _ray, glm::vec3 _intersect, vec3 _LightPos, int depth, std::list<Shared<Sphere>>* Objs, std::list<Shared<Plane>>* Planes);
	vec3 getPos() { return m_position; };
	float getRadius() { return m_radius; };
	vec3 getNormal(vec3 _intersect);

	void setRadius(float _rad) { m_radius = _rad; };
	void setRoughness(float _rough) { Roughness = _rough; };
	int ID;
private:
	
	vec3 albedo = vec3(1.0f, 1.0f, 1.0f);
	
	vec3 F0 = vec3(0.05f);
	float Roughness = 0.5f;
	float m_radius = 0.5f;
	glm::vec3 m_position{0.0f, 0.0f, -8.0f};

	int maxDepth = 1;

	vec3 DiffuseBRDF(vec3 _intersect, vec3 _lightPos);
	vec3 SpecularBRDF(vec3 _intersect, vec3 _lightPos);
	vec3 fresnelSchlick(float cosTheta);
	float GeomSchlickGGX(float NdotV);
	float DistributionGGX(vec3 N, vec3 H);
	float GeomSmith(vec3 N, vec3 V, vec3 L);
};