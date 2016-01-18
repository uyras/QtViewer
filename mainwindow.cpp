#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QPainter>
#include <QMessageBox>
#include <QTableWidget>
#include <QtEvents>
#include <QDebug>
#include "Part.h"
#include "Vect.h"

#include <iostream>
#include <cstring>
#include <string>
#include <cmath>
#include <fstream>
#include <vector>
#include <map>
#include <ctime>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    sysprop(this)
{
    ui->setupUi(this);

    //нопочки и контрллы
    connect(ui->scaler,SIGNAL(valueChanged(int)),ui->surface,SLOT(scaleTo(int)));

    //масштабирование визуальной области
    connect(ui->surface, SIGNAL(scaleUp()), this, SLOT(scaleUp()));
    connect(ui->surface, SIGNAL(scaleDown()), this, SLOT(scaleDown()));

    //изменение состояния магнитной системы
    connect(this, SIGNAL(updateSys()), ui->surface->scene(), SLOT(update()));
    connect(ui->surface->scene(), SIGNAL(systemChanged()), this, SLOT(recalcSystemInfo()));

    //открываем закрываем окно свойств системы
    connect(ui->systemProperties, SIGNAL(triggered(bool)), &sysprop, SLOT(setVisible(bool)));



    //обновляем данные в диалоге свойств
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::saveParticles(){
    StateMachineFree s = sys.state;
    if (sys.Minstate().size()>0)
        sys.setToGroundState();
    if (sys.Maxstate().size()>0)
        sys.setToMaximalState();
    sys.state = s;
	QString filename = QFileDialog::getSaveFileName(this,"Выберите файл для сохранения");
	if (!filename.isEmpty()) {
        sys.save(filename);
	}
}

void MainWindow::loadParticles(QString filename){

    if (filename.isEmpty())
        filename = QFileDialog::getOpenFileName(this,"Выберите конфигурацию файла");

    if (!filename.isEmpty()) {
        sys.load(filename);
        ui->surface->scene()->init(&sys);
    }
    //устанавливаем автоматические коэффициенты
    this->toggleAutoCoff(ui->autoScale->isChecked());
}

void MainWindow::toggleAutoCoff(bool ok)
{
    if (ok){
        //считаем авто коэфф.
        const double normalM = 25.,
                normalSpace=50.;
        double averM=0., minSpace=sys[0]->pos.space(sys[1]->pos);
        Part *temp1, *temp2;
        vector<Part*>::iterator iter1 = sys.parts.begin(), iter2;
        int i=0;
        while (iter1!=sys.parts.end()){
            temp1 = *iter1;
            averM = (averM*(double)i+temp1->m.length())/(double)(i+1);
            iter2=iter1;
            iter2++;
            while (iter2!=sys.parts.end()){
                temp2 = *iter2;
                temp1->pos.space(temp2->pos);
                iter2++;
            }
            iter1++; i++;
        }
        ui->surface->scene()->setCoffs(normalM/averM, normalSpace/minSpace);
    } else {
        //устанавливаем по умлочанию
        ui->surface->scene()->setCoffs(1., 1.);
    }
    emit ui->surface->scene()->updatePoses();
}

void MainWindow::scaleUp(){
    ui->scaler->setValue(ui->scaler->value() + ui->scaler->pageStep());
}

void MainWindow::scaleDown(){
    ui->scaler->setValue(ui->scaler->value() - ui->scaler->pageStep());
}

void MainWindow::recalcSystemInfo()
{
    ui->infoLbl->setText(QString("E=%1").arg(sys.E()));
    emit sysprop.updateData(&sys);
}

void MainWindow::setMinState()
{
    sys.setMinstate(sys.State());
    emit updateSys();
}

void MainWindow::getMinState()
{
    sys.setState(sys.Minstate());
    emit updateSys();
}

void MainWindow::setMaxState()
{
    sys.setMaxstate(sys.State());
    emit updateSys();
}

void MainWindow::getMaxState()
{
    sys.setState(sys.Maxstate());
    emit updateSys();
}

void MainWindow::clearCurrentState()
{
    sys.state.reset();
    emit updateSys();
}
