#include "mainwindow.h"
#include "generatedialog.h"
#include <QApplication>
#include <QTableWidget>
#include <QObject>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	MainWindow w;

	QTableWidget *et = new QTableWidget(&w);
	et->setObjectName("energyTable");
	et->close();


	w.show();
    return a.exec();
}
