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
	//obiektach bufora wierzchołków VBO
	QOpenGLBuffer arrayBuf;
	//obiekt tablic wierzchołków VAO
	QOpenGLVertexArrayObject m_vao;
};

#endif // !MESH_H
