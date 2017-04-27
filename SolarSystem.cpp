#include "Solarsystem.h"

template <typename T>
void remove(std::list<T>& vec, size_t pos) {
	typename std::list<T>::iterator it = vec.begin();
	std::advance(it, pos);
	vec.erase(it);
}

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


	//createPlanet(sunShader, ToStringTexture(SUN));
	qDebug() << nPlanets.size();
	qDebug() << "size";
}

SolarSystem::~SolarSystem() {

}

void SolarSystem::addPlanet(Planet* planet) {
	nPlanets.push_back(planet);
	qDebug() << nPlanets.size();
}

void SolarSystem::deletePlanet(int index) {
	qDebug() << "Delete planet index " << index;
	remove(nPlanets, index);
	qDebug() << nPlanets.size();
}

void SolarSystem::createPlanet(QString name, double radius, double mass, glm::vec3 position, glm::vec3 velocity, SolarSystem::TextureFile texture) {
	Planet* Sun = new Planet(name, 10E10, glm::dvec3(0, 0, 0), 5, 1, 512, sunShader, ToStringTexture(texture));
	Sun->GetObject()->GetPosition().setPosition(position);
	Sun->GetObject()->GetPosition().SetScale(glm::dvec3(radius, radius, radius));
	addPlanet(Sun);
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

