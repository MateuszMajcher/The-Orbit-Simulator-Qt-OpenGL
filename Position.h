#pragma once
#ifndef POSITION_H
#define POSITION_H
#include <iostream>   
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\quaternion.hpp>
#include <glm\detail\precision.hpp>
#include <QDebug>
using glm::dvec3;
using glm::dquat;
class Position {
public:
	Position(const dvec3& pos = dvec3(), const dquat& rot = dquat(), const dvec3& scale = dvec3(1.0, 1.0, 1.0))
	: m_pos(pos), m_rotate(rot), m_scale(scale) {
		qDebug() << pos.x;
	}

	~Position();

private:
	dvec3 m_pos;
	dquat m_rotate;
	dvec3 m_scale;
};

#endif
