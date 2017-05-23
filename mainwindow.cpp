#include "mainwindow.h"

#include <QSlider>
#include <QListWidget>
#include <QDockWidget>
#include <QKeyEvent>
#include <QPushButton>
#include <QToolBar>
#include <QApplication>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QSize>
#include <QTableWidget>
#include <QMessageBox>
#include <QtWidgets>

MainWindow::MainWindow()
	:glWidget(new GLWidget(this))
{
	setWindowTitle("Symulacja ukladu slonecznego");
    setCentralWidget(glWidget);
	createActions();
	createDock();
	//setMinimumSize(200, 200);
}

MainWindow::~MainWindow()
{
	
}

void MainWindow::createActions() {
	QMenu *fileMenu = menuBar()->addMenu(tr("&Akcje"));
	QToolBar *fileToolBar = addToolBar(tr("Akcje"));

	const QIcon runIcon = QIcon::fromTheme("Run", QIcon(":/images/play.png"));
	const QIcon pauseIcon = QIcon::fromTheme("Stop", QIcon(":/images/pause.png"));
	runAct = new QAction(runIcon, tr("Start symulacji"), this);
	pauseAct = new QAction(pauseIcon, tr("Zatrzymanie symulacji"), this);

	runAct->setStatusTip(tr("Start symulacji"));
	pauseAct->setStatusTip(tr("Zatrzymanie symulacji"));
	

	


	startButton = new QToolButton;
	startButton->setDefaultAction(runAct);
	fileToolBar->addWidget(startButton);

	connect(runAct, &QAction::triggered, this, &MainWindow::startSimulation);
	connect(pauseAct, &QAction::triggered, this, &MainWindow::stopSimulation);
	


	const QIcon addIcon = QIcon::fromTheme("Add", QIcon(":/images/add.png"));
	addAct = new QAction(addIcon, tr("Dodaj planete"), this);
	addAct->setShortcuts(QKeySequence::New);
	addAct->setStatusTip(tr("Dodanie planety"));
	
	connect(addAct, &QAction::triggered, this, &MainWindow::addPlanet);
	fileMenu->addAction(addAct);
	fileToolBar->addAction(addAct);



	fileMenu->addSeparator();

	QAction *quitAct = fileMenu->addAction(tr("&Quit"), this, &QWidget::close);
	quitAct->setShortcuts(QKeySequence::Quit);
	quitAct->setStatusTip(tr("Quit the application"));

	


	QMenu *helpMenu = menuBar()->addMenu(tr("&Pomoc"));

	QAction *aboutAct = helpMenu->addAction(tr("&O aplikacji"), this, &MainWindow::about);
	aboutAct->setStatusTip(tr("Opis aplikacji"));

	QAction *aboutQtAct = helpMenu->addAction(tr("Wersja &Qt"), qApp, &QApplication::aboutQt);
	aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
}

void MainWindow::about() {
	QMessageBox::about(this, tr("Symulacja planet"), ("Projekt \n Symulacja komputerowa\n Wykonal Mateusz Majcher"));
}

QSlider *MainWindow::createSlider() {
	QSlider *slider = new QSlider(Qt::Vertical);
	slider->setRange(0, 360 * 16);
	slider->setSingleStep(16);
	slider->setPageStep(15 * 16);
	slider->setTickInterval(15 * 16);
	slider->setTickPosition(QSlider::TicksRight);
	return slider;
}

void MainWindow::createDock() {
	QDockWidget *dock = new QDockWidget(tr("Lista planet"), this);
	dock->setAllowedAreas(Qt::BottomDockWidgetArea);
	dock->setMaximumWidth(400);
	dock->setMaximumHeight(150);

	planetList = new QTableWidget(0, 3, this);
	QTableWidgetItem *nameHeader = new QTableWidgetItem(tr("Nazwa"));
	QTableWidgetItem *radiusHeader = new QTableWidgetItem(tr("Promien"));
	QTableWidgetItem *massHeader = new QTableWidgetItem(tr("Masa"));
	planetList->setHorizontalHeaderItem(0, nameHeader);
	planetList->setHorizontalHeaderItem(1, radiusHeader);
	planetList->setHorizontalHeaderItem(2, massHeader);
	planetList->horizontalHeader()->setStretchLastSection(true);
	planetList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	planetList->setSelectionBehavior(QAbstractItemView::SelectRows);
	planetList->setSelectionMode(QAbstractItemView::SingleSelection);

	dock->setWidget(planetList);
	addDockWidget(Qt::BottomDockWidgetArea, dock);
	//viewMenu->addAction(dock->toggleViewAction());
	/************************************************************************/
	QDockWidget *simulationDock = new QDockWidget(tr("Panel ustawien"), this);
	simulationDock->setAllowedAreas(Qt::BottomDockWidgetArea);
	settingsPanel = new QWidget(simulationDock);


	QGridLayout *layout = new QGridLayout();




	//SLider predkosci
	QSlider* slider = new QSlider(Qt::Orientation::Horizontal);
	slider->setTickPosition(QSlider::TicksBelow);
	slider->setTickInterval(1);
	slider->setSingleStep(1);
	slider->setValue(100);

	QLabel* speedValue = new QLabel("Predkosc symulacji", this);


	//button dodania planety
	createPlanet_button = new QPushButton("Dodaj planete", this);
	//button usuniecia planety
	deletePlanet_button = new QPushButton("Usun", this);
	//button resetu
	Reset_button = new QPushButton("Reset", this);

	//dodanie widgetow do layaotu
	layout->addWidget(createPlanet_button, 0, 0, 1, 1);
	layout->addWidget(deletePlanet_button, 0, 1, 1, 1);
	layout->addWidget(Reset_button, 0, 2, 1, 1);
	layout->addWidget(speedValue, 1, 0, 1, 3);
	layout->addWidget(slider, 1, 1, 1, 3);

	//ustawienie layoautu
	settingsPanel->setLayout(layout);

	simulationDock->setWidget(settingsPanel);
	addDockWidget(Qt::BottomDockWidgetArea, simulationDock);

	connect(createPlanet_button, SIGNAL(clicked()), this, SLOT(addPlanet()));
	connect(deletePlanet_button, SIGNAL(clicked()), this, SLOT(eraseItem()));
	connect(Reset_button, SIGNAL(clicked()), this, SLOT(reset()));
	connect(slider, SIGNAL(valueChanged(int)), glWidget, SLOT(setSpeed(int)));
	connect(glWidget,
		SIGNAL(updateTable(QString, double, double)),
		this,
		SLOT(updateTable(QString, double, double)));
}

/*Usuniecie wiersza tabeli*/
void MainWindow::eraseItem()
{
	int current = planetList->currentRow();
	
	if (current >= 0) {
		emit deletePlanet(current);
		planetList->removeRow(current);
	}
}

/*Dodanie planety*/
void MainWindow::addPlanet() {
	AddDialog* loginDialog = new AddDialog(this);
	connect(loginDialog,

		SIGNAL(acceptData(QString&, double, double, glm::vec3, glm::vec3)),
		glWidget,
		SLOT(createPlanet(QString&, double, double, glm::vec3, glm::vec3)));

	loginDialog->exec();
	qDebug() << "Okno ";
	
	
}
/*Uaktualnienie tabeli*/
void MainWindow::updateTable(QString name, double radius, double mass) {
	planetList->insertRow(planetList->rowCount());
	planetList->setItem(planetList->rowCount() - 1, 0, new QTableWidgetItem(name));
	planetList->setItem(planetList->rowCount() - 1, 1, new QTableWidgetItem(QString::number(radius)));
	planetList->setItem(planetList->rowCount() - 1, 2, new QTableWidgetItem(QString::number(mass)));
}

void MainWindow::startSimulation() {
	if (glWidget->start()) {
		startButton->setDefaultAction(pauseAct);
		addAct->setDisabled(true);
		createPlanet_button->setDisabled(true);
		deletePlanet_button->setDisabled(true);
		Reset_button->setDisabled(true);
	}
}

void MainWindow::stopSimulation() {
	glWidget->stop();
	startButton->setDefaultAction(runAct);
	Reset_button->setDisabled(false);
}

void MainWindow::reset() {
	glWidget->reset();
	addAct->setDisabled(false);
	createPlanet_button->setDisabled(false);
	deletePlanet_button->setDisabled(false);
	planetList->setRowCount(0);
}

void MainWindow::addSun() {
	qDebug() << "dodano slonce";
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
	if (e->key() == Qt::Key_Escape)
		close();
	else
		QWidget::keyPressEvent(e);
}


