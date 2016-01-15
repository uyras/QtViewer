#include "systemproperties.h"
#include "ui_systemproperties.h"

SystemProperties::SystemProperties(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SystemProperties)
{
    ui->setupUi(this);
    connect(ui->maxStateLoadBtn,SIGNAL(clicked(bool)),this,SLOT(getMaxState()));
    connect(ui->maxStateSaveBtn,SIGNAL(clicked(bool)),this,SLOT(setMaxState()));
    connect(ui->minStateLoadBtn,SIGNAL(clicked(bool)),this,SLOT(getMinState()));
    connect(ui->minStateSaveBtn,SIGNAL(clicked(bool)),this,SLOT(setMinState()));
    connect(ui->currentStateClearBtn,SIGNAL(clicked(bool)),this,SLOT(clearCurrentState()));
}

SystemProperties::~SystemProperties()
{
    delete ui;
}

void SystemProperties::updateData()
{
    ui->currentStateLbl->setText(QString::fromStdString(sys->state.toString()));
    ui->minStateLbl->setText(QString::fromStdString(sys->Minstate().toString()));
    ui->maxStateLbl->setText(QString::fromStdString(sys->Maxstate().toString()));
}

void SystemProperties::setMinState()
{
    sys->setMinstate(sys->State());
    emit sysUpdated();
}

void SystemProperties::getMinState()
{
    sys->setState(sys->Minstate());
    emit sysUpdated();
}

void SystemProperties::setMaxState()
{
    sys->setMaxstate(sys->State());
    emit sysUpdated();
}

void SystemProperties::getMaxState()
{
    sys->setState(sys->Maxstate());
    emit sysUpdated();
}

void SystemProperties::clearCurrentState()
{
    sys->state.reset();
    emit sysUpdated();
}
