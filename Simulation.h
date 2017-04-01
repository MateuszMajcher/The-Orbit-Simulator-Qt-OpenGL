#pragma once
#ifndef SIMULATION_H
#define SIMULATION_H
#include "object.h"
#include "Camera.h"
#include <list>


class Simulation {
public:
	Simulation();
	~Simulation();
	static Simulation* CurrentSim;
	static void addObject(Object* obj);
	void drawScene(QOpenGLShaderProgram *program, Camera& camera);

private:
	std::list<Object*> objects;

};

#endif // !SIMULATION_H
