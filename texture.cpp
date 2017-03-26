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
	m_texture->bind();
}

