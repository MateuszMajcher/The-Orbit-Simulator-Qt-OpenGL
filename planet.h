#pragma once

#ifndef PLANET_H
#define PLANET_H
#include <string>
// Include GLM
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "sphere.h"
#include "object.h"
#include "shader.h"
#include "texture.h"


class Planet {
public:
	Planet(QString name, double mass, glm::dvec3 velocity, double angularVelociy, double radius, double lightIntencity, Shader* shader, const QString& pathTexture);
	~Planet();
	void draw(Camera& camera);
	inline void SetObject(Object* object) { m_object = object; m_object->getName() = m_name; }
	Object* GetObject() { return m_object; }

private:
	QString m_name;
	double m_mass;
	double m_radius;
	glm::dvec3 m_velocity;
	double m_spin = 0;
	double m_angularVelocity;
	double m_lightIntencity;
	Object* m_object;
};


#endif
