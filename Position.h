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
using glm::dmat4;
using glm::mat4;
using glm::angleAxis;
class Position {
public:
	Position(const dvec3& pos = dvec3(), const dquat& rot = dquat(), const dvec3& scale = dvec3(1.0, 1.0, 1.0))
	: m_pos(pos), m_rotate(rot), m_scale(scale) {
		qDebug() << pos.x;
	}

	~Position();

	inline mat4 GetModel() const {
		dmat4 position = glm::translate(m_pos);
		dmat4 scale = glm::scale(m_scale);
		dmat4 rotation = mat4_cast(m_rotate);
		return position * rotation * scale;
	}

	inline dvec3& getPosition() { return m_pos; }
	inline void setPosition(const dvec3 pos) { m_pos = pos; }
	inline dquat& getRotate() { return m_rotate; }
	inline void setAngle(const double angle, const dvec3 axis) { m_rotate = angleAxis(angle, axis); }
	inline void SetRot(const dquat rot) { m_rotate = rot; }
	inline dvec3& getScale() { return m_scale; }
	inline void SetScale(const dvec3 scale) { m_scale = scale; }

private:
	dvec3 m_pos;
	dquat m_rotate;
	dvec3 m_scale;
};

#endif
