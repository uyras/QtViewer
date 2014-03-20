#include "generatedialog.h"
#include "ui_generatedialog.h"

generateDialog::generateDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::generateDialog)
{
	ui->setupUi(this);
    emit recalcInfoLabels();
}

generateDialog::~generateDialog()
{
	delete ui;
}

void generateDialog::on_buttonBox_rejected()
{
	this->close();
}

void generateDialog::on_buttonBox_accepted()
{
    emit this->accept();
	close();
}

void generateDialog::recalcInfoLabels()
{
    double w = this->ui->width->value();
    double h = this->ui->height->value();
    double S = w*h;
    double radius = this->ui->radius->value();

    int type = this->ui->genType->currentIndex();
    int count; double concentration;
    switch (type){
    case 0: //случайно по плотности
        concentration = this->ui->partDestiny->value();
        count = ceil((w*h*concentration)/(M_PI*radius*radius));
        break;
    case 1: //случайно по количеству
        count = this->ui->partCount->value();
        concentration = ((double)count*radius*radius*M_PI)/S;
        break;
    case 2: //цепочкой
        double distance = this->ui->partSpacing->value();
        if (distance>0) {
            int cols = floor(w / (2. * radius + distance) );
            int rows = floor(h / (2. * radius + distance) );

            //если влазит еще одна частица, пихаем
            if ( w - cols * (2.*radius+distance) >= 2.*radius){
                cols++;
            }
            if ( h - rows * (2.*radius+distance) >= 2.*radius){
                rows++;
            }
            count = cols*rows;
        }
        else
            count = floor(w / (2.*radius)) * floor (h / (2.*radius));
        concentration = ((double)count*radius*radius*M_PI)/S;
    }

    this->ui->particleCount->setText(QString::number(count));
    this->ui->particleConcentration->setText(QString::number(concentration));
    this->ui->square->setText(QString::number(S));
}

int generateDialog::getW(){
    return ui->width->value();
}

int generateDialog::getH(){
    return ui->height->value();
}

double generateDialog::getR(){
    return ui->radius->value();
}

double generateDialog::getM(){
    return ui->mLength->value() / 100. * ui->radius->value();
}

double generateDialog::getC(){
    return ui->partDestiny->value();
}

int generateDialog::getN(){
    return ui->partCount->value();
}

double generateDialog::getDistance(){
    return ui->partSpacing->value();
}

int generateDialog::getMode(){
    return ui->genType->currentIndex();
}
