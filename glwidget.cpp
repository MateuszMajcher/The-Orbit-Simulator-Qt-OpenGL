#include "glwidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <math.h>
#include <QMessageBox>
#include <ctime>
#include "Solarsystem.h"


// Camera init
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat yaw = -90.0f;	
GLfloat pitch = 0.0f;
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
GLfloat rotx = 0;

//klawiatura
bool keys[1024];
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

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
	, camera(glm::vec3(0.0f, 0.0f, 8.0f))
{
	setFocusPolicy(Qt::StrongFocus);
	setFocus();
	QSurfaceFormat format = QSurfaceFormat::defaultFormat();
	format.setSamples(16);
	format.setSwapInterval(0);
	QSurfaceFormat::setDefaultFormat(format);
	this->setFormat(format);

	//sygnal usuniecia planety
	connect(parent,
		SIGNAL(deletePlanet(int)),
		this,
		SLOT(deletePlanet(int)));


	sh = new Shader(vertex, fragment);
	texture = new Texture(sh);
	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(updaterot()));
	m_timer->start(20);
	
}

/*
Utworznie planety i dodanie do sceny
Update tabeli
*/
void GLWidget::createPlanet(QString& name, double radius, double mass, glm::vec3 position, glm::vec3 velocity) {
	qDebug() << "nazwa: " << name;
	qDebug() << "radius: " << radius;
	qDebug() << "masa: " << mass;
	qDebug() << "pozycja: " << position.x<<position.y<<position.z;
	qDebug() << "ped: " << velocity.x << velocity.y << velocity.z;
	mainScene->createPlanet(name, radius, mass, position, velocity, SolarSystem::SUN);
	emit updateTable(name, radius, mass);
}

/*
Utworzenie ukladu slonecznego 
*/
void GLWidget::createSolarSystem() {
	QString name;
	double radius, mass;

	name = "sun", radius = 2; mass = 1.00000597682;
	mainScene->createPlanet(name, radius, mass, glm::dvec3(0, 0, 0), glm::dvec3(0, 0, 0), SolarSystem::SUN);
	emit updateTable(name, radius, mass);

	name = "Jowisz", radius = 1; mass = 0.000954786104043;
	mainScene->createPlanet(name, radius, mass, glm::dvec3(-3.5023653, -3.8169847, -1.5507963), glm::dvec3(0.00565429, -0.00412490, -0.00190589), SolarSystem::JUPITER);
	emit updateTable(name, radius, mass);

	name = "Saturn", radius = 1.8; mass = 0.000285583733151;
	mainScene->createPlanet(name, radius, mass, glm::dvec3(9.0755314, -3.0458353, -1.6483708), glm::dvec3(0.00168318, 0.00483525, 0.00192462), SolarSystem::SATURN);
	emit updateTable(name, radius, mass);

	name = "Uran", radius = 1.4; mass = 0.0000437273164546;
	mainScene->createPlanet(name, radius, mass, glm::dvec3(8.3101420, -16.2901086, -7.2521278), glm::dvec3(0.00354178, 0.00137102, 0.00055029), SolarSystem::URANUS);
	emit updateTable(name, radius, mass);

	name = "Neptun", radius = 1.8; mass = 0.0000517759138449;
	mainScene->createPlanet(name, radius, mass, glm::dvec3(11.4707666, -25.7294829, -10.8169456), glm::dvec3(0.00288930, 0.00114527, 0.00039677), SolarSystem::NEPTUNE);
	emit updateTable(name, radius, mass);


	
	
}

//delete planety ze sceny
void GLWidget::deletePlanet(int idx) {
	mainScene->deletePlanet(idx);
	qDebug() << "usunieto planet nr: " << idx;
}

//ustawienie predkosci symulacji
void GLWidget::setSpeed(int speed) {
	mainScene->setTime(speed);
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


//Uruchomienie symulacji
bool GLWidget::start() {
	qDebug() << "Start";
	qDebug() << mainScene->getNumberOfPlanets();
	
	if (mainScene->getNumberOfPlanets() >= 2) {
		mainScene->start();
		return true;
	}
	else {
		int ret = QMessageBox::warning(this, tr("Nie mozna rozpoczac symulacji"),
			tr("Dodaj co najmniej 2 planety.\n"),
			QMessageBox::Ok);
		return false;
	}
	
}

//zatrzymanie symulacji
void GLWidget::stop() {
	qDebug() << "Stop";
	mainScene->stop();
}

//zrestartowanie symulacji
void GLWidget::reset() {
	qDebug() << "Reset";
	mainScene->reset();
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


	//obj = new Object("mars", m_geometry, texture);
	Position pos;
	planet = new Planet("Sun", 10E10, glm::dvec3(0, 0, 0), 5, 1, 512, sh, "texture/texture_sun.jpg");

	mainScene = new SolarSystem();
	createSolarSystem();
}



void GLWidget::paintGL()
{
	//set frame time
	GLfloat currentTime = std::clock();
	deltaTime = currentTime - lastFrame;
	lastFrame = currentTime;
	deltaTime = deltaTime / (float)CLOCKS_PER_SEC;
	//qDebug() << deltaTime;


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	Do_Movement();



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
	//planet->draw(camera);
	mainScene->Update(rotx);
	mainScene->drawPlanet(camera);

	// wy³¹czenie shadera
	sh->getProgram()->release();
}

void GLWidget::resizeGL(int w, int h)
{
	camera.setSize(w, h);
}

void GLWidget::Do_Movement() {
	// Camera controls
	if (keys[Qt::Key_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[Qt::Key_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[Qt::Key_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[Qt::Key_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
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

void GLWidget::keyPressEvent(QKeyEvent* e) {

	keys[e->key()] = true;
}


void GLWidget::keyReleaseEvent(QKeyEvent* e) {
	keys[e->key()] = false;
}