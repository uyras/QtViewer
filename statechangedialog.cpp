#include "statechangedialog.h"
#include "ui_statechangedialog.h"

StateChangeDialog::StateChangeDialog(QWidget *parent, QString title, QString oldValue) :
    QDialog(parent),
    ui(new Ui::StateChangeDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(title);
    ui->state->setPlainText(oldValue);
}

StateChangeDialog::~StateChangeDialog()
{
    delete ui;
}

QString StateChangeDialog::getState()
{
    return ui->state->toPlainText();
}
