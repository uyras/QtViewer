#include "systemproperties.h"
#include "ui_systemproperties.h"
#include "mainwindow.h"

SystemProperties::SystemProperties(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SystemProperties),
    p(parent)
{
    ui->setupUi(this);

    //обрабатываем события кнопок в окне свойств системы
    connect(ui->maxStateLoadBtn,SIGNAL(clicked(bool)), parent, SLOT(getMaxState()));
    connect(ui->maxStateSaveBtn,SIGNAL(clicked(bool)), parent, SLOT(setMaxState()));
    connect(ui->minStateLoadBtn,SIGNAL(clicked(bool)), parent, SLOT(getMinState()));
    connect(ui->minStateSaveBtn,SIGNAL(clicked(bool)), parent, SLOT(setMinState()));
    connect(ui->currentStateClearBtn,SIGNAL(clicked(bool)), parent, SLOT(clearCurrentState()));
    connect(ui->interactionRangeSaveBtn,SIGNAL(clicked()), this, SLOT(changeInteractionRange()));
}

SystemProperties::~SystemProperties()
{
    delete ui;
}

void SystemProperties::updateData(PartArray *sys)
{

    ui->currentStateLbl->setText(split(QString::fromStdString(sys->state.toString())));
    ui->minStateLbl->setText(split(QString::fromStdString(sys->Minstate().toString())));
    ui->maxStateLbl->setText(split(QString::fromStdString(sys->Maxstate().toString())));
    ui->eMinLbl->setText(QString::number(sys->EMin()));
    ui->eMaxLbl->setText(QString::number(sys->EMax()));
    ui->partsNumberLabel->setText(QString::number(sys->size()));
    ui->interactionRangeField->setValue(sys->interactionRange());
}

void SystemProperties::changeInteractionRange()
{
    QMessageBox msgBox;
    msgBox.setText("В результате действия будут удалены минимальная и максимальная энергии.");
    msgBox.setInformativeText("Вы хотите продолжить?");
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    MainWindow *mw = qobject_cast<MainWindow*>(p);
    if (ret==QMessageBox::Yes){
        mw->setInteractionRange(this->ui->interactionRangeField->value());
    } else {
        emit mw->updateSys();
    }
}

QString SystemProperties::split(const QString &s)
{
    QString pathNameClean(s);

    int c = pathNameClean.length();
    int n=30; //каждые сколько символов разбивать на подстроки

    if( c > n)
    {
        for(int i = 1; i <= c/n; i++)
        {
            pathNameClean.insert(i * n, " ");
        }
    }
    return pathNameClean;
}
