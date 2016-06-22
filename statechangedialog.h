#ifndef STATECHANGEDIALOG_H
#define STATECHANGEDIALOG_H

#include <QDialog>

namespace Ui {
class StateChangeDialog;
}

class StateChangeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StateChangeDialog(QWidget *parent, QString title, QString oldValue);
    ~StateChangeDialog();
    QString getState();

private:
    Ui::StateChangeDialog *ui;
};

#endif // STATECHANGEDIALOG_H
