#ifndef OBJECT_H
#define OBJECT_H
#include <string>
#include "mesh.h"
#include "texture.h"

class Object : protected QOpenGLFunctions {
public:
	Object(const QString& name);
	Object(const QString& name, Mesh* obj, Texture* tex);
	void Draw(QOpenGLShaderProgram *program);
	~Object();

private:
	Mesh* mesh;
	Texture* tex;
	QString name;
};

#endif // !OBJECT_H
