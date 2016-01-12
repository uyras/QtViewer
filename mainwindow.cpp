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
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //нопочки и контрллы
    connect(ui->scaler,SIGNAL(valueChanged(int)),ui->surface,SLOT(scaleTo(int)));

    //масштабирование визуальной области
    connect(ui->surface, SIGNAL(scaleUp()), this, SLOT(scaleUp()));
    connect(ui->surface, SIGNAL(scaleDown()), this, SLOT(scaleDown()));

    this->Parts = ui->surface->scene();

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::paintEvent(QPaintEvent *pe){
    Q_UNUSED(pe);
}

void MainWindow::emptyENFolder(){
	QDir dir("en");
	if (dir.exists()){
		dir.removeRecursively();
	}
	dir.cd(QDir::currentPath());
	dir.mkdir("en");
}

void MainWindow::saveParticles(){

	QString filename = QFileDialog::getSaveFileName(this,"Выберите файл для сохранения");

	if (!filename.isEmpty()) {
		std::string s = filename.toStdString();
		char * cfilename = new char [s.length()+1];
		std::strcpy (cfilename, s.c_str());

        this->Parts->save(cfilename);
	}
}

void MainWindow::loadParticles(QString filename){

    if (filename.isEmpty())
        filename = QFileDialog::getOpenFileName(this,"Выберите конфигурацию файла");

    if (!filename.isEmpty()) {
        this->Parts->load(filename);
	}
}

void MainWindow::clearParticles(){
    Parts->clear();
}

void MainWindow::setMState(double m){
	mState->setText("m=" + QString().setNum(m));
}

void MainWindow::setE1State(double e1){
	mState->setText("E1=" + QString().setNum(e1));
}

void MainWindow::setE2State(double e2){
	mState->setText("E2=" + QString().setNum(e2));
}

void MainWindow::scaleSystem(){
    //Parts->scaleSystem(ui->resizeDelimiter->value());
}

void MainWindow::scaleUp(){
    ui->scaler->setValue(ui->scaler->value() + ui->scaler->pageStep());
}

void MainWindow::scaleDown(){
    ui->scaler->setValue(ui->scaler->value() - ui->scaler->pageStep());
}
