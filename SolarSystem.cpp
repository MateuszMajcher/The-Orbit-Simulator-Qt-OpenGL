#include "Solarsystem.h"

template <typename T>
void remove(std::list<T>& vec, size_t pos) {
	typename std::list<T>::iterator it = vec.begin();
	std::advance(it, pos);
	vec.erase(it);
}

template <typename T>
void remove(T& vec, size_t pos) {
	typename T::iterator it = vec.begin();
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



}

SolarSystem::~SolarSystem() {

}

void SolarSystem::addPlanet(Planet* planet) {
	nPlanets.push_back(planet);
	qDebug() << "Liczba planet" << nPlanets.size();
}

void SolarSystem::deletePlanet(int index) {
	qDebug() << "Delete planet index " << index;
	remove(nPlanets, index);
	remove(masses, index);
	remove(q, index);
	remove(p, index);
	for (Planet* planet : nPlanets) {
		qDebug() << planet->getName();
	}

	for (double m : masses) {
		qDebug() << m;
	}

	for (point_type m : q) {
		qDebug() << m;
	}

	qDebug() <<"Liczba planet" << nPlanets.size();
}

void SolarSystem::createPlanet(QString name, double radius, double mass, glm::dvec3 position, glm::dvec3 velocity, SolarSystem::TextureFile texture) {
	Planet* Sun = new Planet(name, 10E10, glm::dvec3(0, 0, 0), 5, 1, 512, sunShader, ToStringTexture(texture));
	Sun->GetObject()->GetPosition().setPosition(position);
	Sun->GetObject()->GetPosition().SetScale(glm::dvec3(radius, radius, radius));
	qDebug() << "masa:";
	masses.push_back(mass);
	p.push_back(point_type(velocity));
	q.push_back(point_type(position));

	qDebug() << "masa: " << masses[masses.size() - 1];
	qDebug() << "pozycja: " << q[masses.size() - 1];
	qDebug() << "velotiy: " << p[masses.size() - 1];

	addPlanet(Sun);
}

void SolarSystem::start() {
	qDebug() << "rozpoczecie symulacji";
	qDebug() << init;

	//run - czy calkowac
	//init - czy inicialiowac stan zmiennych

	run = true;
	if (!init) {
		init = true;

		qmean = center_of_mass(q, masses);
		pmean = center_of_mass(p, masses);
		qDebug() << "Srednia q: " << qmean;
		qDebug() << "Srednia p: " << pmean;

		for (size_t i = 0; i < q.size(); ++i)
		{
			q[i] -= qmean;
			p[i] -= pmean;
			qDebug() << "srednia1: " << q[i];
			qDebug() << "srednia2: " << p[i];
		}

		for (size_t i = 0; i < q.size(); ++i) {
			p[i] *= masses[i];
			qDebug() << "masa: " << p[i] << endl;
		}
	}

}

int SolarSystem::getNumberOfPlanets() {
	return nPlanets.size();
}

void SolarSystem::stop() {
	qDebug() << "zatrzymanie symulacji";
	run = false;
}

void SolarSystem::reset() {
	qDebug() << "reset";
	nPlanets.clear();
	masses.clear();
	q.clear();
	p.clear();
	init = false;
	t = 0;
	qDebug() << "nPlanet size " <<nPlanets.size();
	qDebug() << "masses size " << masses.size();
	qDebug() << "q size " << q.size();
	qDebug() << "p size " << p.size();
}

void SolarSystem::setTime(double time) {
	dt = time;
	qDebug() << "Speed " << dt;
}

void SolarSystem::Update(GLfloat rotx) {
	if (run) {
		stepper_type().do_step(make_pair(solar_system_coor(masses), solar_system_momentum(masses)),
			make_pair(boost::ref(q), boost::ref(p)), t, dt);
			t += dt;

		int i = 0;
		for (Planet* p : nPlanets) {
			Position pos;
			//pos.setAngle(rotx, glm::vec3(1.f, 0.f, 0.f));
			//p->GetObject()->GetPosition().setAngle(rotx, glm::vec3(1.f, 0.f, 0.f));
			p->GetObject()->GetPosition().setPosition(q[i].getVector());
			i++;
		}

		qDebug() << t;
		for (size_t i = 0; i < q.size(); ++i) {
			qDebug() << "\n"; qDebug() << q[i].getVector().x;
			qDebug() << q[i].getVector().y;
			qDebug() << q[i].getVector().z;
			qDebug() << "\n";
		}

	}
}

QString SolarSystem::ToStringTexture(SolarSystem::TextureFile v) {
	switch (v) {
	case SUN: return "texture/texture_sun.jpg";
	case EARTH: return "texture/texture_earth.jpg";
	case MARS: return "texture/texture_mars.jpg";
	case JUPITER: return "texture/texture_jupiter.jpg";
	case VENUS: return "texture/texture_venus.jpg";
	case SATURN: return "texture/texture_saturn.jpg";
	case PLUTO: return "texture/texture_pluto.jpg";
	case NEPTUNE: return "texture/texture_neptune.jpg";
	case URANUS: return "texture/texture_uranus.jpg";
	case OTHER: return "texture/metal.png";
	default:  return "texture/metal.png";
	};
};

void SolarSystem::drawPlanet(Camera& camera) {
	for (Planet* planet : nPlanets) {
		planet->draw(camera);
	}
}

SolarSystem::point_type SolarSystem::center_of_mass(const container_type &x, const mass_type &m) {
	double overall_mass = 0.0;
	point_type mean(0.0);
	for (size_t i = 0; i < x.size(); ++i) {
		overall_mass += m[i];
		mean += m[i] * x[i];
	}
	if (!x.empty()) mean /= overall_mass;
	return mean;
}

double SolarSystem::energy(const container_type &q, const container_type &p, const mass_type &masses) {
	const size_t n = q.size();
	double en = 0.0;
	for (size_t i = 0; i<n; ++i)
	{
		en += 0.5 * norm(p[i]) / masses[i];
		for (size_t j = 0; j<i; ++j)
		{
			double diff = abs(q[i] - q[j]);
			en -= gravitational_constant * masses[j] * masses[i] / diff;
		}
	}
	return en;
}

