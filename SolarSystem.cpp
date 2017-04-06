#include "Solarsystem.h"

SolarSystem::SolarSystem() 
:sunShader(new Shader(vertex, fragment)), planetShader(new Shader(vertex, fragment)) {
	createPlanet(sunShader, ToStringTexture(SUN));
	qDebug() << nPlanets.size();
}

SolarSystem::~SolarSystem() {

}

void SolarSystem::addPlanet(Planet* planet) {
	nPlanets.push_back(planet);
}

void SolarSystem::createPlanet(Shader* shader,const QString& path) {
	Planet* Sun = new Planet("Sun", 10E10, glm::dvec3(0, 0, 0), 5, 1, 512, shader, path);
	Sun->GetObject()->GetPosition().setPosition(glm::dvec3(0, 0, 0));
}

QString SolarSystem::ToStringTexture(SolarSystem::TextureFile v) {
	switch (v) {
	case SUN: return "texture/texture_sun.jpg";
	case EARTH: return "texture/texture_sun.jpg";
	case MARS: return "texture/texture_sun.jpg";
	default:  return "[Unknown OS_type]";
	};
};