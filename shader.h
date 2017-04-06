#pragma once
#ifndef SHADER_H
#define SHADER_H
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include "Position.h"
#include "Camera.h"
// Include GLM
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1
#define PROGRAM_NORMAL_ATTRIBUTE 2

class Camera;
class Position;

class Shader : protected QOpenGLFunctions {
public:
	Shader(const char* vertex, const char* fragment);
	~Shader();
	void Bind();
	QOpenGLShaderProgram *getProgram();
	void addVertex(const char* vertex);
	void addFragment(const char* fragment);
	void setup();
	void update(Position& position, Camera& camera);
	void SetColor(glm::vec4 color);
	void SetEmission(float emission);
private:
	QOpenGLShaderProgram *m_program;
	GLuint m_uniforms[5];
	std::string m_name;

	enum MyEnum
	{
		MODEL_MATRIX_U,
		VIEW_MATRIX_U,
		PROJECTION_MATRIX_U,
		COLOR_U,
		EMISSION_U
	};
};

#endif 