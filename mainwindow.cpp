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
	this->Parts = new PartArray();

    //диалоговое окно
    gd = new generateDialog(this);
    gd->setObjectName("generateDialog");
    QObject::connect(gd,SIGNAL(accepted()),this,SLOT(generate()));

	//нопочки и контрллы
    connect(this,SIGNAL(drawParts(PartArray*)),ui->surface->scene(),SLOT(fullReDraw(PartArray*)));
    connect(this,SIGNAL(reDrawParts(PartArray*)),ui->surface->scene(),SLOT(reDraw(PartArray*)));
    connect(ui->scaler,SIGNAL(valueChanged(int)),ui->surface,SLOT(scaleTo(int)));
	connect(ui->resizeSystem,SIGNAL(clicked()),this,SLOT(scaleSystem())); //масштабирование системы
	connect(ui->showH,SIGNAL(clicked()),this,SLOT(calcH()));//кнопка "показать H"

	//пересчет параметров при изменении системы
	connect(this,SIGNAL(drawParts(PartArray*)),SLOT(recalcParameters(PartArray*)));
	connect(this,SIGNAL(reDrawParts(PartArray*)),SLOT(recalcParameters(PartArray*)));

	//статус-бар
	mState = new QLabel(this);
	e1State = new QLabel(this);
	e2State = new QLabel(this);
	ui->statusBar->addWidget(mState);
	ui->statusBar->addWidget(e1State);
	ui->statusBar->addWidget(e2State);
    connect(ui->surface->scene(),SIGNAL(setE1(double)),this,SLOT(setE1State(double)));
    connect(ui->surface->scene(),SIGNAL(setE2(double)),this,SLOT(setE2State(double)));
    connect(ui->surface->scene(),SIGNAL(setM(double)),this,SLOT(setMState(double)));

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::recalcParameters(){
	Vect temp;
	Parts->calcEnergy1();
	ui->eVal->setText(QString::number(Parts->E1));

	temp = Parts->calcM1();
	ui->m1Val->setText(QString("{%1 , %2} , %3").arg(temp.x).arg(temp.y).arg(temp.length()));

	temp = Parts->calcM2();
	ui->m2Val->setText(QString("{%1 , %2} , %3").arg(temp.x).arg(temp.y).arg(temp.length()));

	ui->j1Val->setText(QString::number(Parts->calcJ()));
	ui->j2Val->setText(QString::number(Parts->calcJ2()));

	ui->countVal->setText(QString::number(Parts->count()));

	ui->cVal->setText(QString::number(Parts->destiny()));

	//считаем H системы и отн. H системы
	Vect h1,h2;
    vector<Part*>::iterator iter1,iter2;
	iter1 = Parts->parts.begin();
	while (iter1!=Parts->parts.end()){
		iter2 = Parts->parts.begin();
		while (iter2!=Parts->parts.end()){
			if (iter2!=iter1){
                temp = (*iter1)->interact((*iter2));
				h1.x += temp.x;
				h1.y += temp.y;
				h1.z += temp.z;

				h2.x += fabs(temp.x);
				h2.y += fabs(temp.y);
				h2.z += fabs(temp.z);
			}
			iter2++;
		}
		iter1++;
	}
	h2.x = h1.x/h2.x;
	h2.y = h1.y/h2.y;
	h2.z = h1.z/h2.z;

	ui->h1Val->setText(QString("{%1 , %2} , %3").arg(h1.x).arg(h1.y).arg(h1.length()));
	ui->h2Val->setText(QString("{%1 , %2} , %3").arg(h2.x).arg(h2.y).arg(h2.length()));
}

void MainWindow::recalcParameters(PartArray* p){
	Q_UNUSED(p);
	recalcParameters();
}

void MainWindow::on_surface_destroyed()
{

}

void MainWindow::initParts()
{
	this->Parts->calcEnergy1FastIncrementalFirst();

	this->ui->stateNum->setValue(0);

	this->ui->stateNum->setMaximum((int)pow(2.,(float)this->Parts->count())-1);
}

void MainWindow::paintEvent(QPaintEvent *pe){
	Q_UNUSED(pe);
	/*
	QPainter p(ui->surface);

	const int scale = this->ui->scaler->value(), r = 0.5 * scale;

	//рисуем границу
	p.fillRect(0,0,Parts->size.x*scale,Parts->size.y*scale,Qt::white);

	vector<Part>::iterator
			begin = this->Parts->parts.begin(),
			end = this->Parts->parts.end(),
			first = end,
			second = end,
			iter = begin;

	//рисуем кружки
	p.setPen(QPen(Qt::red,1,Qt::SolidLine));
	int i=0;
	while (iter != this->Parts->parts.end()){

		//определяем координаты конца и начала связи
		if (!startDot.isNull() && !endDot.isNull()){
			if ((startDot - QPoint(iter->pos.x * scale,iter->pos.y * scale)).manhattanLength() <= r)
				first = iter;

			if ((endDot - QPoint(iter->pos.x * scale,iter->pos.y * scale)).manhattanLength() <= r)
				second = iter;
		}

		if ( ( ((int)std::pow(2.,(double)i)) & this->Parts->state)==0 )
			p.setPen(Qt::green);
		else
			p.setPen(Qt::red);

		if ( iter==first || iter == second )
			p.setBrush(QBrush(QColor(0,255,0,150),Qt::DiagCrossPattern));
		else
			p.setBrush(QBrush(Qt::NoBrush));

		p.drawEllipse(
					iter->pos.x*scale - r,
					iter->pos.y*scale - r,
					r*2,r*2);
		p.drawLine(
					iter->pos.x * scale,
					iter->pos.y * scale,
					(iter->pos.x+iter->m.x) * scale,
					(iter->pos.y+iter->m.y) * scale
					);

		iter++;
		i++;
	}

	if (!startDot.isNull() && !endDot.isNull()){
		p.drawLine(startDot,endDot);
		if (first!=second && first != end && second != end){
			QString ss;
			ss.append("E=");
			Vect h = second->interact(first.base());
			ss.append(QString::number( h.scalar(first->m) ) );

			p.setBackgroundMode(Qt::OpaqueMode);
			p.drawText((startDot+endDot)/2,ss);
			p.setBackgroundMode(Qt::TransparentMode);
		}

		//если выделили всего одну частицу, дорисовать векторы взаимодействия её на другие
		if (first==second && first != end){
			Vect h1;
			p.setPen(Qt::red);
			iter=begin;
			while (iter != end){
				if (first!=iter){
					h1 = iter->interact(first.base());

					p.drawLine(
								iter->pos.x * scale,
								iter->pos.y * scale,
								(iter->pos.x+h1.x) * scale,
								(iter->pos.y+h1.y) * scale
								);
				}
				iter++;
			}
		}

		//рисуем координаты частицы
		if (first==second && first != end){
			p.setPen(Qt::black);
			QString ss;
			ss.append("{");
			ss.append(QString::number(first->pos.x));
			ss.append(":");
			ss.append(QString::number(first->pos.y));
			ss.append("}");

			p.setBackgroundMode(Qt::OpaqueMode);
			p.drawText(startDot+QPoint(0,10),ss);
			p.setBackgroundMode(Qt::TransparentMode);
		}
	}
	*/
}

void MainWindow::on_stateNum_valueChanged(unsigned long long int arg1)
{
	//this->Parts->transform(arg1);

	//emit reDrawParts(Parts);
}

void MainWindow::on_appendState_clicked()
{
    //this->Parts->transform(this->ui->stateNum->value());
	this->Parts->calcEnergy1Fast();
	this->ui->eVal->setText(QString::number(Parts->E1));

	emit reDrawParts(Parts);
}

void MainWindow::on_pushButton_2_clicked()
{
	QTableWidget *et = this->findChild<QTableWidget *>("energyTable",Qt::FindDirectChildrenOnly);
	et->setWindowFlags(Qt::Window);
	et->setColumnCount(Parts->count());
	et->setRowCount(Parts->count());

	//et->set;

	//получаем элементы в таблицу
    vector<Part*>::iterator iter = this->Parts->parts.begin();

	std::vector<double>::iterator eIter;
	int i=0,j;

	while (iter != this->Parts->parts.end()){
        eIter = (*iter)->eArray.begin();
		j=0;
        while (eIter!=(*iter)->eArray.end()){
			et->setItem(i,j,new QTableWidgetItem(QString::number(*eIter)));
			eIter++;
			j++;
		}
		iter++;
		i++;
	}

	et->showMaximized();
}

void MainWindow::on_genBtn_clicked()
{
    this->gd->open();
}

void MainWindow::generate()
{
    config::Instance()->partR = gd->getR();
    config::Instance()->m = gd->getM();
    Parts->resize(gd->getW(), gd->getH(), 1); //изменяем размеры системы (параметры чистятся автоматически)
    this->ui->surface->scene()->clearMousePointers();
    switch (gd->getMode()){
    case 0: //случайно по плотности
        Parts->dropRandom(gd->getC());
        break;
    case 1: //случайно по количеству
        Parts->dropRandom(gd->getN());
        break;
    case 2: //цепочка по расстоянию
        Parts->dropChain(gd->getDistance() + gd->getR()*2.);
    }

	initParts();
	emit drawParts(Parts);
}

void MainWindow::emptyENFolder(){
	QDir dir("en");
	if (dir.exists()){
		dir.removeRecursively();
	}
	dir.cd(QDir::currentPath());
	dir.mkdir("en");
}

void MainWindow::on_pushButton_4_clicked()
{
	map<double, unsigned int> eTable;
	map<double, ofstream*> fileTable;


	map<double, unsigned int>::iterator eIter;
	map<double, ofstream*>::iterator fileIter;

	//чистим папку энергий
	emptyENFolder();

	ofstream f("eDegen.dat");
	ofstream *fTemp;

	srand(time(NULL)); //устанавливаем начальную точку генератора случайных чисел

	int n = Parts->count();

	Parts->calcEnergy1FastIncrementalFirst(); //предподготовка к просчёту

	unsigned long long int state;
	const unsigned long long int maxstate =  pow(2.,n-1);
	double eTemp;
	state=0;
	do {

		//считаем энергию
		eTemp = Parts->calcEnergy1FastIncremental(state);
        eTemp = (double)(int)(eTemp*1000) / 1000.; //обрезаем все кроме последнего знака после точки

		//записываем энергию
		eTable[eTemp] = eTable[eTemp] + 1;

		//записываем номер состояния в соотв. файл
		fileIter = fileTable.find(eTemp);
		if (fileIter == fileTable.end()){
			char c[100];
			sprintf(c,"en\\%f.dat",eTemp);
			fTemp = new std::ofstream(c);
			fileTable.insert ( std::pair<double, ofstream*>(eTemp,fTemp) );
		} else
			fTemp = fileIter->second;
		(*fTemp) << state << endl;

		state++;

	} while (state<maxstate);

	//сохраняем в файл

	eIter = eTable.begin();
	while (eIter!=eTable.end()){
		f << eIter->first << "\t" << eIter->second << endl;
		eIter++;
	}

	//закрываем файлы состояний
	fileIter = fileTable.begin();
	while (fileIter!=fileTable.end()){
		fileIter->second->close();
		fileIter++;
	}

	eTable.clear();
	fileTable.clear();
	f.close();

}

void MainWindow::saveParticles(){
	this->ui->stateNum->setValue(0);

	QString filename = QFileDialog::getSaveFileName(this,"Выберите файл для сохранения");

	if (!filename.isEmpty()) {
		std::string s = filename.toStdString();
		char * cfilename = new char [s.length()+1];
		std::strcpy (cfilename, s.c_str());

		this->Parts->save(cfilename);
		initParts();
	}
}

void MainWindow::loadParticles(){
	this->ui->stateNum->setValue(0);

	QString filename = QFileDialog::getOpenFileName(this,"Выберите конфигурацию файла");

	if (!filename.isEmpty()) {
		std::string s = filename.toStdString();
		char * cfilename = new char [s.length()+1];
		std::strcpy (cfilename, s.c_str());

		this->Parts->load(cfilename);
		initParts();

		emit drawParts(Parts);
	}
}

void MainWindow::clearParticles(){
	Parts->clear();
	emit drawParts(Parts);
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
	Parts->scaleSystem(ui->resizeDelimiter->value());
	emit drawParts(Parts);
}

void MainWindow::on_pushButton_clicked()
{
	Parts->setMAllUp();
	emit drawParts(Parts);
}

void MainWindow::calcH(){
    Parts->calcH();
	emit drawParts(Parts);
}
