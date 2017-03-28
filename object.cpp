#include "object.h"

Object::Object(const QString& name) {
	this->name = name;
}

Object::Object(const QString& name, Mesh* mesh, Texture* tex) {
	this->name = name;
	this->mesh = mesh;
	this->tex = tex;
}

void Object::Draw(QOpenGLShaderProgram *program) {
	tex->bind();
	mesh->draw(program);
}

Object::~Object() {

}

