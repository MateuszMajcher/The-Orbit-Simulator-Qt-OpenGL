#include "planet.h"


Planet::Planet(QString name, double mass, glm::dvec3 velocity, double angularVelocity, double radius, double lightIntencity, Shader* shader, const QString& pathTexture)
   :m_name(name), m_mass(mass), m_velocity(velocity), m_angularVelocity(angularVelocity), m_radius(radius), m_lightIntencity(lightIntencity) {

	Sphere* m_geometry = new Sphere();
	Texture* tex = new Texture(shader);
	tex->addTexture(pathTexture);
	SetObject(new Object(name, m_geometry, tex));
	GetObject()->GetPosition().SetScale(glm::dvec3(radius));
}

Planet::~Planet() {
	delete m_object;
}


void Planet::draw(QOpenGLShaderProgram *program, Camera& camera) {
	glCullFace(GL_BACK);      // pomija rysowanie œcian odwróconych ty³em
	m_object->Draw(program, camera);
}