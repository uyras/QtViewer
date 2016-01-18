#include "systemproperties.h"
#include "ui_systemproperties.h"
#include "mainwindow.h"

SystemProperties::SystemProperties(QWidget *parent) :
    QWidget(0),
    ui(new Ui::SystemProperties)
{
    ui->setupUi(this);

    //обрабатываем события кнопок в окне свойств системы
    connect(ui->maxStateLoadBtn,SIGNAL(clicked(bool)), parent, SLOT(getMaxState()));
    connect(ui->maxStateSaveBtn,SIGNAL(clicked(bool)), parent, SLOT(setMaxState()));
    connect(ui->minStateLoadBtn,SIGNAL(clicked(bool)), parent, SLOT(getMinState()));
    connect(ui->minStateSaveBtn,SIGNAL(clicked(bool)), parent, SLOT(setMinState()));
    connect(ui->currentStateClearBtn,SIGNAL(clicked(bool)), parent, SLOT(clearCurrentState()));
}

SystemProperties::~SystemProperties()
{
    delete ui;
}

void SystemProperties::updateData(PartArray *sys)
{
    ui->currentStateLbl->setText(QString::fromStdString(sys->state.toString()));
    ui->minStateLbl->setText(QString::fromStdString(sys->Minstate().toString()));
    ui->maxStateLbl->setText(QString::fromStdString(sys->Maxstate().toString()));
}
