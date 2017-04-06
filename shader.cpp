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

	//qDebug() << m_uniforms[MODEL_MATRIX_U];
}

void Shader::update(Position& position, Camera& camera) {

	glm::mat4 model = position.GetModel();
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = camera.GetProjection();

	m_program->setUniformValue("model", QMatrix4x4(glm::value_ptr(model)).transposed());
	m_program->setUniformValue("view", QMatrix4x4(glm::value_ptr(view)).transposed());
	m_program->setUniformValue("projection", QMatrix4x4(glm::value_ptr(projection)).transposed());

	m_program->setUniformValue("texture", 0);
}

void Shader::SetColor(glm::vec4 color) {
	glUniform4fv(m_uniforms[COLOR_U], 1, glm::value_ptr(color));
}

void Shader::SetEmission(float emission) {
	glUniform1f(m_uniforms[EMISSION_U], emission);
}


void Shader::Bind() {
	m_program->bind();
}

QOpenGLShaderProgram *Shader::getProgram() {
	return m_program;
}