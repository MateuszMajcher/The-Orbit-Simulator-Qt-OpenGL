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
    setCentralWidget(glWidget);
	createActions();
	createDock();
	//setMinimumSize(200, 200);
}

MainWindow::~MainWindow()
{
	
}

void MainWindow::createActions() {
	QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
	QToolBar *fileToolBar = addToolBar(tr("File"));

	const QIcon runIcon = QIcon::fromTheme("Run", QIcon(":/images/run.png"));
	QAction *runAct = new QAction(runIcon, tr("Start symulacji"), this);
	runAct->setShortcuts(QKeySequence::Refresh);
	runAct->setStatusTip(tr("Start symulacji"));
	connect(runAct, &QAction::triggered, glWidget, &GLWidget::start);
	fileMenu->addAction(runAct);
	fileToolBar->addAction(runAct);


	const QIcon addIcon = QIcon::fromTheme("Add", QIcon(":/images/add.png"));
	QAction *addAct = new QAction(addIcon, tr("Dodaj planete"), this);
	addAct->setShortcuts(QKeySequence::New);
	addAct->setStatusTip(tr("Dodanie planety"));
	connect(addAct, &QAction::triggered, this, &MainWindow::addPlanet);
	fileMenu->addAction(addAct);
	fileToolBar->addAction(addAct);


	const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
	QAction *saveAct = new QAction(saveIcon, tr("&Save..."), this);
	saveAct->setShortcuts(QKeySequence::Save);
	saveAct->setStatusTip(tr("Save the current form letter"));
	//connect(saveAct, &QAction::triggered, this, &MainWindow::save);
	fileMenu->addAction(saveAct);
	fileToolBar->addAction(saveAct);



	fileMenu->addSeparator();

	QAction *quitAct = fileMenu->addAction(tr("&Quit"), this, &QWidget::close);
	quitAct->setShortcuts(QKeySequence::Quit);
	quitAct->setStatusTip(tr("Quit the application"));

	QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
	QToolBar *editToolBar = addToolBar(tr("Edit"));
	const QIcon undoIcon = QIcon::fromTheme("edit-undo", QIcon(":/images/undo.png"));
	QAction *undoAct = new QAction(undoIcon, tr("&Undo"), this);
	undoAct->setShortcuts(QKeySequence::Undo);
	undoAct->setStatusTip(tr("Undo the last editing action"));
	//connect(undoAct, &QAction::triggered, this, &MainWindow::undo);
	editMenu->addAction(undoAct);
	editToolBar->addAction(undoAct);


	QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

	QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::about);
	aboutAct->setStatusTip(tr("Show the application's About box"));

	QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
	aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
}

void MainWindow::about() {
	QMessageBox::about(this, tr("Symulacja planet"), (""));
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
	QDockWidget *dock = new QDockWidget(tr("List Planet"), this);
	dock->setAllowedAreas(Qt::BottomDockWidgetArea);
	dock->setMaximumWidth(400);
	dock->setMaximumHeight(150);

	planetList = new QTableWidget(0, 3, this);
	QTableWidgetItem *nameHeader = new QTableWidgetItem(tr("Name"));
	QTableWidgetItem *radiusHeader = new QTableWidgetItem(tr("Radius"));
	QTableWidgetItem *massHeader = new QTableWidgetItem(tr("Mass"));
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
	QDockWidget *simulationDock = new QDockWidget(tr("settings panel"), this);
	simulationDock->setAllowedAreas(Qt::BottomDockWidgetArea);
	settingsPanel = new QWidget(simulationDock);


	QGridLayout *layout = new QGridLayout();


	//Sterowanie

	//SLider predkosci
	QSlider* slider = new QSlider(Qt::Orientation::Horizontal);
	slider->setTickPosition(QSlider::TicksBothSides);
	slider->setTickInterval(1);
	slider->setSingleStep(1);
	slider->setValue(50);

	//button dodania planety
	QPushButton* createPlanet_button = new QPushButton("Dodaj planete", this);
	//button usuniecia planety
	QPushButton* deletePlanet_button = new QPushButton("Usun", this);
	//button resetu
	QPushButton* Reset_button = new QPushButton("Reset", this);

	//dodanie widgetow do layaotu
	layout->addWidget(createPlanet_button, 0, 0, 1, 1);
	layout->addWidget(deletePlanet_button, 0, 1, 1, 1);
	layout->addWidget(Reset_button, 0, 2, 1, 1);
	layout->addWidget(slider, 1, 0, 1, 3);

	//ustawienie layoautu
	settingsPanel->setLayout(layout);

	simulationDock->setWidget(settingsPanel);
	addDockWidget(Qt::BottomDockWidgetArea, simulationDock);

	connect(createPlanet_button, SIGNAL(clicked()), this, SLOT(addPlanet()));
	connect(deletePlanet_button, SIGNAL(clicked()), this, SLOT(eraseItem()));
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

void MainWindow::updateTable(QString name, double radius, double mass) {
	planetList->insertRow(planetList->rowCount());
	planetList->setItem(planetList->rowCount() - 1, 0, new QTableWidgetItem(name));
	planetList->setItem(planetList->rowCount() - 1, 1, new QTableWidgetItem(QString::number(radius)));
	planetList->setItem(planetList->rowCount() - 1, 2, new QTableWidgetItem(QString::number(mass)));
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


