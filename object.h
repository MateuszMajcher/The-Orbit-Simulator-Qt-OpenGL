#ifndef OBJECT_H
#define OBJECT_H
#include <string>
#include "triangle.h"
#include "texture.h"

class Object : protected QOpenGLFunctions {
public:
	Object(const QString& name);
	Object(const QString& name, Mesh* obj, Texture* tex);
	void Draw();
	~Object();
};

#endif // !OBJECT_H
