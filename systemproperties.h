#ifndef SYSTEMPROPERTIES_H
#define SYSTEMPROPERTIES_H

#include <QWidget>
#include "PartArray.h"

namespace Ui {
class SystemProperties;
}

class SystemProperties : public QWidget
{
    Q_OBJECT

public:
    explicit SystemProperties(QWidget *parent = 0);
    ~SystemProperties();
    PartArray *sys;

signals:
    void sysUpdated();

public slots:
    void updateData();

    //операции с минимумом
    void setMinState();
    void getMinState();
    void setMaxState();
    void getMaxState();
    void clearCurrentState();

private:
    Ui::SystemProperties *ui;
};

#endif // SYSTEMPROPERTIES_H
