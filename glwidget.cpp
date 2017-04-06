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
GLfloat rotx = 0;

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



GLWidget::GLWidget(QWidget *parent)
	: QOpenGLWidget(parent)
	, m_program(0)
	, m_geometry(0)
	, camera(glm::vec3(0.0f, 0.0f, 3.0f))
{
	QSurfaceFormat format = QSurfaceFormat::defaultFormat();
	format.setSamples(16);
	format.setSwapInterval(0);
	QSurfaceFormat::setDefaultFormat(format);
	this->setFormat(format);
	sh = new Shader(vertex, fragment);
	texture = new Texture(sh);
	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(updaterot()));
	m_timer->start(20);
	
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
	delete mainScene;
	m_program = 0;
	doneCurrent();
}

void GLWidget::updaterot() {
	rotx+=0.1;
	update();
}


void GLWidget::initializeGL()
{

	connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);

	initializeOpenGLFunctions();
	glClearColor(0, 0, 0, m_transparent ? 0 : 1);
	
	//m_geometry = new Sphere();
	sh->addVertex(vertex);
	sh->addFragment(fragment);
	sh->setup();

	//utworzenie  vao
	//Generowanie identyfikatorów obiektów tablic wierzcho³ków
	m_vao.create();
	QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
	mainScene = new SolarSystem();

	//obj = new Object("mars", m_geometry, texture);
	Position pos;
	planet = new Planet("Sun", 10E10, glm::dvec3(0, 0, 0), 5, 1, 512, sh, "texture/texture_sun.jpg");
}

/*void drawScene(Simulation& scene, Camera camera) {
	scene.drawScene(camera);
}*/

void GLWidget::paintGL()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	//glm::quat &rot = glm::angleAxis(glm::radians(rotx), glm::vec3(1.f, 0.f, 0.f));
	pos.setAngle(rotx, glm::vec3(1.f, 0.f, 0.f));
	//pos->SetRot(rotx);
	glm::dvec3 &posx = glm::dvec3(rotx/50, 0.f, 0.f);
	pos.setPosition(posx);
	glm::dvec3 &scalex = glm::dvec3(1, 1.f, 1.f);
	pos.SetScale(scalex);

	m_vao.bind();
	QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
	
	//obj->SetPosition(pos);
	planet->GetObject()->SetPosition(pos);
	//narysowanie danych zawartych w tablicach wierzcho³ków dla obiektu
	//obj->Draw(sh->getProgram(), camera);
	planet->GetObject()->Draw(sh->getProgram(), camera);
	// wy³¹czenie shadera
	sh->getProgram()->release();
}

void GLWidget::resizeGL(int w, int h)
{
	camera.setSize(w, h);
}

void GLWidget::mousePressEvent(QMouseEvent *e)
{
	lastX = e->pos().x();
	lastY = e->pos().y();
	
	//qDebug() << lastX;
}

bool firstMouse = true;

void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
	if (e->MouseButtonPress) {
	
		GLfloat xoffset = (GLfloat)e->x() - lastX;
		GLfloat yoffset = lastY - (GLfloat)e->y(); // Reversed since y-coordinates go from bottom to left
		lastX = (GLfloat)e->x();
		lastY = (GLfloat)e->y();
		camera.ProcessMouseMovement(xoffset, yoffset);
	}
	update();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *e)
{
	e = e;
}

void GLWidget::wheelEvent(QWheelEvent* e) {
	QPoint numPixels = e->pixelDelta();
	QPoint numDegrees = e->angleDelta() / 8;
	QPoint numSteps = numDegrees / 15;
	//qDebug() << numSteps;
	camera.ProcessMouseScroll(numSteps.y());
}

