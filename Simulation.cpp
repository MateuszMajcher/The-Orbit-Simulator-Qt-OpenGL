#pragma once
#include "Simulation.h"

Simulation* Simulation::CurrentSim;

Simulation::Simulation() {}

Simulation::~Simulation() {}

void Simulation::addObject(Object* obj) {
	Simulation::CurrentSim->objects.push_back(obj);
}

void Simulation::drawScene(Camera& camera) {
	for (std::list<Object*>::iterator v = objects.begin(); v != objects.end(); v++) {
		(*v)->Draw(camera);
	}
}
