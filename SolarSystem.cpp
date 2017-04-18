#include "Solarsystem.h"

SolarSystem::SolarSystem() 
{
	sunShader = new Shader(vertex, fragment);
	sunShader->addVertex(vertex);
	sunShader->addFragment(fragment);
	sunShader->setup();
	planetShader = new Shader(vertex, fragment);
	planetShader->addVertex(vertex);
	planetShader->addFragment(fragment);
	planetShader->setup();


	createPlanet(sunShader, ToStringTexture(SUN));
	qDebug() << nPlanets.size();
	qDebug() << "size";
}

SolarSystem::~SolarSystem() {

}

void SolarSystem::addPlanet(Planet* planet) {
	nPlanets.push_back(planet);
	qDebug() << nPlanets.size();
}

void SolarSystem::createPlanet(Shader* shader, const QString& path) {
	Planet* Sun = new Planet("Sun", 10E10, glm::dvec3(0, 0, 0), 5, 1, 512, shader, path);
	Sun->GetObject()->GetPosition().setPosition(glm::dvec3(0, 0, 0));

	Planet* mars = new Planet("Mars", 10E10, glm::dvec3(0, 0, 0), 5, 1, 512, shader, ToStringTexture(SolarSystem::MARS));
	mars->GetObject()->GetPosition().setPosition(glm::dvec3(0, 1, 0));

	addPlanet(Sun);
	addPlanet(mars);
}

void SolarSystem::Update(GLfloat rotx) {
	for (Planet* p : nPlanets) {
		Position pos;
		pos.setAngle(rotx, glm::vec3(1.f, 0.f, 0.f));
		p->GetObject()->GetPosition().setAngle(rotx, glm::vec3(1.f, 0.f, 0.f));
	}
}

QString SolarSystem::ToStringTexture(SolarSystem::TextureFile v) {
	switch (v) {
	case SUN: return "texture/texture_sun.jpg";
	case EARTH: return "texture/texture_sun.jpg";
	case MARS: return "texture/texture_mars.jpg";
	default:  return "[Unknown OS_type]";
	};
};

void SolarSystem::drawPlanet(Camera& camera) {
	for (Planet* planet : nPlanets) {
		planet->draw(camera);
	}
}