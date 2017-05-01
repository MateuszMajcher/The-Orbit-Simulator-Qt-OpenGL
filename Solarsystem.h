#pragma once
#ifndef SOLARSYSTEM_H
#define SOLARSYSTEM_H

#include <list>
#include "planet.h"
// Include GLM
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "point_type.h"
#include <boost/array.hpp>
#include <boost/numeric/odeint.hpp>

using namespace std;
using namespace boost::numeric::odeint;

class Planet;
enum TextureFile;

static const char *vertex = R"(
   #version 330 core

	attribute vec3 vertex;
attribute vec2 in_texcoord;
varying vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(vertex, 1.0f);
    TexCoord = vec2(in_texcoord.x, 1.0 - in_texcoord.y);
}
)";

static const char *fragment = R"(
   #version 330 core

			 varying  vec2 TexCoord;

			out vec4 color;

			uniform sampler2D ourTexture1;

			void main()
{
    color = texture(ourTexture1, TexCoord);
}
)";

struct mass_type;
const double gravitational_constant = 2.95912208286e-4;


class SolarSystem {

private:
	std::list<Planet*> nPlanets;
	//Shader slonca
	Shader* sunShader;
	//Shader planet
	Shader* planetShader;
	//Symulacja
	bool run = false;
	//obliczenia
	bool init = false;


	//Boost odeint
	//size_t n = 2;
	
	typedef point<double, 3> point_type;
	typedef std::vector< point_type> container_type;
	typedef std::vector<double> mass_type;
	typedef symplectic_rkn_sb3a_mclachlan< container_type > stepper_type;


	mass_type masses;

	container_type q;
	container_type p;

	point_type qmean;
	point_type pmean;


	const double dt = 100.0;
	double t = 0.0;

public:
	SolarSystem();
	~SolarSystem();
	 
	enum TextureFile {
			SUN,
			EARTH,
			MARS
		};

	QString ToStringTexture(SolarSystem::TextureFile v);
	// Dodanie planety do sceny
	void addPlanet(Planet* planet);
	//usuniecie planety ze sceny
	void deletePlanet(int index);
	// Utworzenie nowej planety
	void createPlanet(QString name, double radius, double mass, glm::dvec3 position, glm::dvec3 velocity, SolarSystem::TextureFile texture);
	//Rusowanie sceny
	void drawPlanet(Camera& camera);
	//update sceny
	void Update(GLfloat rotx);
	//rozpoczecie symulacji
	void start();
	//reset symulacji
	void reset();


	//boost
	struct solar_system_coor {
		const mass_type &m_masses;

		solar_system_coor(const mass_type &masses) : m_masses(masses) { }

		void operator()(const container_type &p, container_type &dqdt) const
		{
			size_t n = p.size();
			for (size_t i = 0; i<n; ++i)
				dqdt[i] = p[i] / m_masses[i];
		}

	};


	// [momentum_function
	struct solar_system_momentum
	{
		const mass_type &m_masses;

		solar_system_momentum(const mass_type &masses) : m_masses(masses) { }

		void operator()(const container_type &q, container_type &dpdt) const
		{
			const size_t n = q.size();
			for (size_t i = 0; i<n; ++i)
			{
				dpdt[i] = 0.0;
				for (size_t j = 0; j<i; ++j)
				{
					point_type diff = q[j] - q[i];
					double d = abs(diff);
					diff *= (gravitational_constant * m_masses[i] * m_masses[j] / d / d / d);
					dpdt[i] += diff;
					dpdt[j] -= diff;

				}
			}
		}
	};
	//]


	//[ streaming_observer
	struct streaming_observer
	{
		std::ostream& m_out;

		streaming_observer(std::ostream &out) : m_out(out) { }

		template< class State >
		void operator()(const State &x, double t) const
		{
			container_type &q = x.first;
			m_out << t;
			for (size_t i = 0; i<q.size(); ++i) m_out << "\t" << q[i];
			m_out << "\n";
		}
	};
	//]

	point_type center_of_mass(const container_type &x, const mass_type &m);

	double energy(const container_type &q, const container_type &p, const mass_type &masses);

};


#endif