#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow mainWindow;
	mainWindow.resize(mainWindow.sizeHint());

	int desktopArea = QApplication::desktop()->width() * QApplication::desktop()->height();

	mainWindow.show();
	return a.exec();
}
