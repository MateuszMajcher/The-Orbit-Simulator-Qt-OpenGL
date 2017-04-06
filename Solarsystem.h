#pragma once
#ifndef SOLARSYSTEM_H
#define SOLARSYSTEM_H

#include <vector>
#include "planet.h"

class Planet;
enum TextureFile;

static const char *vertex = R"(
   #version 330 core

	attribute vec3 vertex;
attribute vec2 in_texcoord;
varying vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(vertex, 1.0f);
    TexCoord = vec2(in_texcoord.x, 1.0 - in_texcoord.y);
}
)";

static const char *fragment = R"(
   #version 330 core

			 varying  vec2 TexCoord;

			out vec4 color;

			uniform sampler2D ourTexture1;

			void main()
{
    color = texture(ourTexture1, TexCoord);
}
)";


class SolarSystem {
public:
	SolarSystem();
	~SolarSystem();
	 
	enum TextureFile {
			SUN,
			EARTH,
			MARS
		};

	QString ToStringTexture(SolarSystem::TextureFile v);
	// Dodanie planety do sceny
	void addPlanet(Planet* planet);
	// Utworzenie nowej planety
	void createPlanet(Shader* shader, const QString& path);

private:
	std::vector<Planet*> nPlanets;
	//Shader slonca
	Shader* sunShader;
	//Shader planet
	Shader* planetShader;
};


#endif