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

    if (argc>1)
        emit w.loadParticles(QString::fromLocal8Bit(argv[1]));

	w.show();
    return a.exec();
}
