#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <glm/glm.hpp>
#include <vector>
#include "mesh.h"


class VertexData;
class Sphere : public Mesh
{
public:
	Sphere();
	 ~Sphere();

	void draw(QOpenGLShaderProgram *program);

private:
	void init();

	QVector4D color;

	QOpenGLBuffer indexBuf;

	unsigned int m_meridians;
	unsigned int m_latitudes;
	std::vector<VertexData> m_vertices;
	std::vector<unsigned short> m_indices;
};

#endif // TRIANGLE_H