#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H
#include <QOpenGLTexture>
#include "shader.h"
#include "Position.h"
#include "Camera.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
using glm::vec3;
using glm::mat4;

class Shader;
class Position;
class Camera;

class Texture {
public:
	Texture(Shader* shader);
	~Texture();
	void addTexture(const QString& path);
	void bind();
	void Update(Position& position, Camera& cam);

	void SetColor(glm::vec4 color) { m_color = color; }
	glm::vec4& GetColor() { return m_color; }
	void SetEmission(float emission) { m_emission = emission; }
	Shader* GetShader() { return m_shader; }

private:
	Shader* m_shader;
	QOpenGLTexture* m_texture;
	glm::vec4 m_color;
	float m_emission;
};
#endif