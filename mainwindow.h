#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QHBoxLayout>
#include "glwidget.h"

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

};

#endif // MAINWINDOW_H
