#ifndef MESH_H
#define MESH_H
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class Mesh : protected QOpenGLFunctions {
public:
	virtual void draw(QOpenGLShaderProgram *program) = 0;
private:
	virtual void init() = 0;
protected:
	QOpenGLBuffer arrayBuf;

};

#endif // !MESH_H
