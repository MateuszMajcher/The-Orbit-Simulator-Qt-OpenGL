#include "texture.h"

Texture::Texture(Shader* shader) {
	this->m_shader = shader;
}

Texture::~Texture() {
	delete m_texture;
}

void Texture::addTexture(const QString& path) {
	m_texture = new QOpenGLTexture(QImage(path).mirrored(true, true));
	m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
	m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
}

void Texture::bind() {
	if (m_shader != nullptr) {
		m_shader->Bind();
		//m_shader->SetColor(m_color);
		//m_shader->SetEmission(m_emission);
	}
	m_texture->bind();
}

void Texture::Update(Position& position, Camera& camera) {
	if (m_shader != NULL) {
		m_shader->update(position, camera);
	}
}

