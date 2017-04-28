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


	

	mass_type masses = { {
			1.00000597682 ,      // sun
			0.000954786104043 ,  // jupiter
		0.000285583733151 ,  // saturn
		0.0000437273164546 , // uranus
		0.0000517759138449 , // neptune
		1.0 / (1.3e8)      // pluto
		} };

	container_type q = { {
			point_type(0.0 , 0.0 , 0.0) ,                        // sun
			point_type(-3.5023653 , -3.8169847 , -1.5507963) ,   // jupiter
		point_type(9.0755314 , -3.0458353 , -1.6483708) ,    // saturn
		point_type(8.3101420 , -16.2901086 , -7.2521278) ,   // uranus
		point_type(11.4707666 , -25.7294829 , -10.8169456) , // neptune
		point_type(-15.5387357 , -25.2225594 , -3.1902382)   // pluto
		} };

	container_type p = { {
			point_type(0.0 , 0.0 , 0.0) ,                        // sun
			point_type(0.00565429 , -0.00412490 , -0.00190589) , // jupiter
		point_type(0.00168318 , 0.00483525 , 0.00192462) ,   // saturn
		point_type(0.00354178 , 0.00137102 , 0.00055029) ,   // uranus
		point_type(0.00288930 , 0.00114527 , 0.00039677) ,   // neptune
		point_type(0.00276725 , -0.00170702 , -0.00136504)   // pluto
		} };

	point_type qmean = center_of_mass(q, masses);
	point_type pmean = center_of_mass(p, masses);
	for (size_t i = 0; i<n; ++i)
	{
		q[i] -= qmean;
		p[i] -= pmean;
	}

	for (size_t i = 0; i<n; ++i) p[i] *= masses[i];

	//[ integration_solar_system



	const double dt = 100;
	double t = 0.0;
	for (size_t i = 0; i<2000; ++i, t += dt)
	{
		//std::this_thread::sleep_for(std::chrono::milliseconds(20));
		stepper_type().do_step(make_pair(solar_system_coor(masses), solar_system_momentum(masses)),
			make_pair(boost::ref(q), boost::ref(p)), t, dt);
		qDebug() << t;
		for (size_t i = 0; i < q.size(); ++i) qDebug() << "\t"; qDebug() << q[1];
		qDebug() << "\n";
	}

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

