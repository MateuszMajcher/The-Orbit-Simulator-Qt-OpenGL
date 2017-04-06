#ifndef MESH_H
#define MESH_H
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>


class Mesh : protected QOpenGLFunctions {
public:
	virtual void draw(QOpenGLShaderProgram *program) = 0;
private:
	virtual void init() = 0;
protected:
	//obiektach bufora wierzcho�k�w VBO
	QOpenGLBuffer arrayBuf;
	//obiekt tablic wierzcho�k�w VAO
	QOpenGLVertexArrayObject m_vao;
};

#endif // !MESH_H
