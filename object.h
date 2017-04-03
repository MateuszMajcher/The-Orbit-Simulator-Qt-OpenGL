#ifndef OBJECT_H
#define OBJECT_H
#include <string>
#include "mesh.h"
#include "texture.h"
#include "Position.h"

class Object : protected QOpenGLFunctions {
public:
	Object(const QString& name);
	Object(const QString& name, Mesh* obj, Texture* tex);
	void Draw(QOpenGLShaderProgram *program, Camera camera);
	~Object();

	Position& GetPosition() { return pos; }
	void SetPosition(Position& position) { pos = position; }
	Texture* GetTexture() { return tex; }
	void SetTexture(Texture* material) { tex = material; }
	QString& getName() { return name; }


private:
	Mesh* mesh;
	Texture* tex;
	Position pos;
	QString name;
};

#endif // !OBJECT_H
