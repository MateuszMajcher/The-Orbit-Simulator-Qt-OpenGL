#include "shader.h"

Shader::Shader(const char* vertex, const char* fragment) :m_program(new QOpenGLShaderProgram) {
}

Shader::~Shader() {
	delete m_program;
}

void Shader::addVertex(const char* vertex) {
	m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertex);
}

void Shader::addFragment(const char* fragment) {
	m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragment);
}

void Shader::setup() {

	m_program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
	m_program->bindAttributeLocation("in_texcoord", PROGRAM_TEXCOORD_ATTRIBUTE);
	m_program->link();
	m_program->bind();


	m_uniforms[PROJECTION_MATRIX_U] = m_program->uniformLocation("projection");
	m_uniforms[VIEW_MATRIX_U] = m_program->uniformLocation("view");
	m_uniforms[MODEL_MATRIX_U] = m_program->uniformLocation("model");
	m_uniforms[COLOR_U] = m_program->uniformLocation("color");
	m_uniforms[EMISSION_U] = m_program->uniformLocation("emmision");

	qDebug() << m_uniforms[MODEL_MATRIX_U];
}


void Shader::Bind() {
	m_program->bind();
}

QOpenGLShaderProgram *Shader::getProgram() {
	return m_program;
}