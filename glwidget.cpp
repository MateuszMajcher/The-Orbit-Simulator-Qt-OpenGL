#include "glwidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <math.h>
#include <QMessageBox>


// Camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat yaw = -90.0f;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
GLfloat pitch = 0.0f;
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;

static const char *vertexShaderSourceCore = R"(
    #version 120
    uniform mat4 projMatrix;
    uniform mat4 mvMatrix;

	    attribute vec4 vertex;
    attribute vec2 in_texcoord;

			    varying vec2 v_texcoord;

			    void main() {
       gl_Position = projMatrix * mvMatrix * vertex;
       v_texcoord = in_texcoord;
    }
)";

static const char *fragmentShaderSourceCore = R"(
    #version 120
    uniform sampler2D texture;

			    varying vec2 v_texcoord;

			    void main() {
       gl_FragColor = texture2D(texture, v_texcoord);
    }
)";

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

GLWidget::GLWidget(QWidget *parent)
	: QOpenGLWidget(parent)
	, m_program(0)
	, m_geometry(0)
{
	QSurfaceFormat format = QSurfaceFormat::defaultFormat();
	format.setSamples(16);
	format.setSwapInterval(0);
	QSurfaceFormat::setDefaultFormat(format);
	this->setFormat(format);
	sh = new Shader(vertex, fragment);
	texture = new Texture(sh);
	m_timer = new QTimer(this);

}

GLWidget::~GLWidget()
{
	cleanup();
}

QSize GLWidget::minimumSizeHint() const
{
	return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
	return QSize(800, 600);
}

static void qNormalizeAngle(int &angle)
{
	while (angle < 0)
		angle += 360 * 16;
	while (angle > 360 * 16)
		angle -= 360 * 16;
}




void GLWidget::cleanup()
{
	makeCurrent();
	delete m_program;
	delete sh;
	m_program = 0;

	doneCurrent();
}




void GLWidget::initializeGL()
{

	connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);

	initializeOpenGLFunctions();
	glClearColor(0, 0, 0, m_transparent ? 0 : 1);
	
	m_geometry = new Sphere();
	sh->addVertex(vertex);
	sh->addFragment(fragment);
	sh->setup();
	//wczytanie programu cieniujacego
	/*m_program = new QOpenGLShaderProgram;
	m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertex);
	m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragment);
	//m_program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
	//m_program->bindAttributeLocation("in_texcoord", PROGRAM_TEXCOORD_ATTRIBUTE);
	m_program->link();
	m_program->bind();*/


	//utworzenie vbo i vao
	//Generowanie identyfikator�w obiekt�w tablic wierzcho�k�w
	m_vao.create();
	QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

	//texture->addTexture("texture/texture_sun.jpg");
	texture->addTexture("texture/metal.png");
	/*m_texture = new QOpenGLTexture(QImage("texture/texture_sun.jpg").mirrored(true, true));
	m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
	m_texture->setMagnificationFilter(QOpenGLTexture::Linear);*/

}

void GLWidget::setupVertexAttribs()
{


}

void GLWidget::paintGL()
{

	m_proj.setToIdentity();
	m_proj.perspective(45.0f, GLfloat(this->width()) / this->height(), 0.01f, 100.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	model = glm::rotate(model, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	projection = glm::perspective(45.0f, (GLfloat)this->width() / (GLfloat)this->height(), 0.1f, 100.0f);
	// Get their uniform location
	/*GLint modelLoc = glGetUniformLocation(ourShader.program, "model");
	GLint viewLoc = glGetUniformLocation(ourShader.program, "view");
	GLint projLoc = glGetUniformLocation(ourShader.program, "projection");
	// Pass them to the shaders
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	// Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));*/

	m_vao.bind();

	QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
	sh->Bind();
	sh->getProgram()->setUniformValue("model", QMatrix4x4(glm::value_ptr(model)).transposed());
	sh->getProgram()->setUniformValue("view", QMatrix4x4(glm::value_ptr(view)).transposed());
	sh->getProgram()->setUniformValue("projection", QMatrix4x4(glm::value_ptr(projection)).transposed());
	texture->bind();
	// Texture unit 0
	sh->getProgram()->setUniformValue("texture", 0);

	//narysowanie danych zawartych w tablicach wierzcho�k�w dla obiektu
	m_geometry->draw(sh->getProgram());

	// wy��czenie shadera
	sh->getProgram()->release();
}

void GLWidget::resizeGL(int w, int h)
{
	m_proj.setToIdentity();
	m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);
}

void GLWidget::mousePressEvent(QMouseEvent *e)
{
	lastX = e->pos().x();
	lastY = e->pos().y();
	
	qDebug() << lastX;
}

bool firstMouse = true;

void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
	if (e->MouseButtonPress) {
	
	GLfloat xoffset = (GLfloat)e->x() - lastX;
	GLfloat yoffset = lastY - (GLfloat)e->y(); // Reversed since y-coordinates go from bottom to left
	lastX = (GLfloat)e->x();
	lastY = (GLfloat)e->y();
	qDebug() << lastX;
	GLfloat sensitivity = 0.05;	// Change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
	}


	update();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *e)
{
	e = e;
}
