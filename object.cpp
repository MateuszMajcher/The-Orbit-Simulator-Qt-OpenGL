#include "object.h"

Object::Object(const QString& name) {
	this->name = name;
}

Object::Object(const QString& name, Mesh* mesh, Texture* tex) {
	this->name = name;
	this->mesh = mesh;
	this->tex = tex;
}

void Object::Draw(QOpenGLShaderProgram *program, Camera camera) {
	tex->bind();
	tex->Update(pos, camera);
	mesh->draw(tex->GetShader()->getProgram());
}

Object::~Object() {

}

