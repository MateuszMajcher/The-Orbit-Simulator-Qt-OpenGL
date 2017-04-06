
#define _USE_MATH_DEFINES
#include <cmath>
#include "sphere.h"

struct VertexData
{
	glm::vec3 position;
	glm::vec2 texCoord;
};

Sphere::Sphere() :
	indexBuf(QOpenGLBuffer::IndexBuffer)
{
	initializeOpenGLFunctions();

	// Generate 2 VBOs
	arrayBuf.create();
	indexBuf.create();

	// 
	init();

	//color = QVector4D(1, 0, 0, 1);
}

Sphere::~Sphere()
{
	arrayBuf.destroy();
	indexBuf.destroy();
}

void Sphere::init()
{
	m_meridians = 30;
    m_latitudes = 30;
	m_vertices.reserve((m_meridians + 1) * (m_latitudes + 2));
	for (size_t i = 0; i < m_meridians + 1; i++)
	{
		for (size_t j = 0; j < m_latitudes + 2; j++)
		{
			// texCoord in the range [(0,0), (1,1)]
			glm::vec2 texCoord((float)i / m_meridians, (float)j / (m_latitudes + 1));
			// theta = longitude from 0 to 2pi
			// phi = latitude from -pi/2 to pi/2
			double theta, phi;
			theta = 2 * M_PI * texCoord.x;
			phi = M_PI * texCoord.y - M_PI_2;
			glm::vec3 pos;
			pos.y = 0.2 * (float)std::sin(phi);
			pos.x = 0.2 *  (float)std::cos(phi) * std::cos(theta);
			pos.z = 0.2 * (float)std::cos(phi) * std::sin(theta);

			m_vertices.push_back({ pos, texCoord });
		}
	}

	// Calculate triangle indices

	for (size_t i = 0; i < m_meridians; i++)
	{
		// Construct triangles between successive meridians
		for (size_t j = 0; j < m_latitudes + 1; j++)
		{
			m_indices.push_back(i * (m_latitudes + 2) + j);
			m_indices.push_back(i * (m_latitudes + 2) + j + 1);
			m_indices.push_back((i + 1) * (m_latitudes + 2) + j + 1);

			m_indices.push_back((i + 1) * (m_latitudes + 2) + j + 1);
			m_indices.push_back((i + 1) * (m_latitudes + 2) + j);
			m_indices.push_back(i * (m_latitudes + 2) + j);

		}
	}

	// Create a vertex array object. In OpenGL ES 2.0 and OpenGL 2.x
	// implementations this is optional and support may not be present
	// at all. Nonetheless the below code works in all cases and makes
	// sure there is a VAO when one is needed.
	m_vao.create();
	QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

	//Obiekty bufora wierzcho³ków
	//Utworzenie obiektu tablic wierzcho³ków
	arrayBuf.create();
	arrayBuf.bind();
	arrayBuf.allocate(m_vertices.data(), m_vertices.size() * sizeof(VertexData));

	indexBuf.create();
	indexBuf.bind();
	indexBuf.allocate(m_indices.data(), m_indices.size() * sizeof(GLushort));
}

void Sphere::draw(QOpenGLShaderProgram *program)
{
	
	QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

	//w³¹czenie shadera
	program->bind();

	//pobranie indeksu atrybutu wierzcho³ka o nazwie "vertex"
	int vertexLocation = program->attributeLocation("vertex");
	////w³¹czenie tablic wierzcho³ków
	program->enableAttributeArray(vertexLocation);
	//zdefiniowanie tablicy wierzcho³ków
	program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(VertexData));

	//pobranie indeksu atrybutu wierzcho³ka o nazwie "in_texcoord"
	int textureLocation = program->attributeLocation("in_texcoord");
	//w³¹czenie tablic wierzcho³ków
	program->enableAttributeArray(textureLocation);
	//zdefiniowanie tablicy wierzcho³ków
	program->setAttributeBuffer(textureLocation, GL_FLOAT, sizeof(glm::vec3), 2, sizeof(VertexData));

	// narysowanie danych zawartych w tablicach wierzcho³ków
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_SHORT, 0);
}

