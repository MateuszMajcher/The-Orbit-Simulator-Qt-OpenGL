#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QHBoxLayout>
#include "glwidget.h"
#include "addDialog.h"


class QSlider;
class QPushButton;
class GLWidget;
class MainWindow;
class QAction;
class QDockWidget;
class QMenu;
class QTextEdit;
class QListWidget;
class QTableWidget;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
	~MainWindow();
	

	
protected:
	void keyPressEvent(QKeyEvent *event) override;

private slots:
	void addPlanet();
	void addSun();
	void eraseItem();
	void updateTable(QString, double, double);
signals:
	void deletePlanet(int);

private:
	void createActions();
	QSlider *createSlider();
	void createDock();
	

	void about();

	GLWidget *glWidget;
	QSlider *xSlider;
	QSlider *ySlider;
	QSlider *zSlider;

	MainWindow *mainWindow;
	QWidget *settingsPanel;

	QMenu *viewMenu;

	QTableWidget *planetList;
	QStringList  m_TableHeader;

};

#endif // MAINWINDOW_H
