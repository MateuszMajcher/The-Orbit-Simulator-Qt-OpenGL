#pragma once
#ifndef GLWIDGET_H
#define GLWIDGET_H


#include <QOpenGLFunctions>
#include <QDialog>
#include <QTimer>
#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include "sphere.h"
#include "shader.h"
#include "texture.h"
#include "object.h"
#include "Camera.h"
#include "Position.h"
#include "planet.h"
// Include GLM
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

using glm::vec3;
using glm::mat4;

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	GLWidget(QWidget *parent = 0);
	~GLWidget();

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

	public slots:
	void cleanup();
	void updaterot();

signals:
	void xRotationChanged(int angle);
	void yRotationChanged(int angle);
	void zRotationChanged(int angle);

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int width, int height) override;
	void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
	void wheelEvent(QWheelEvent* e) Q_DECL_OVERRIDE;

private:
	void setupVertexAttribs();

	QTimer* m_timer;

	QPoint m_lastPos;
	QOpenGLShaderProgram *m_program;
	QOpenGLVertexArrayObject m_vao;

	Mesh *m_geometry;

	
	int m_projMatrixLoc;
	int m_mvMatrixLoc;
	int m_normalMatrixLoc;

	Camera camera;
	
	Texture* texture;
	Shader* sh;

	//przykladowy object
	Object* obj;
	QVector3D m_cameraVelocity;
	Position pos;
	Planet* planet;

	int m_lightPosLoc;
	//mouse
	glm::vec2 mousePressPosition;

	QMatrix4x4 m_view;
	QMatrix4x4 m_proj;
	QMatrix4x4 m_model;


	bool m_transparent;
};

#endif
