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
    explicit SystemProperties(QWidget *parent);
    ~SystemProperties();

signals:
    void sysUpdated();

public slots:
    void updateData(PartArray *sys); //обновить все данные в форме

private:
    Ui::SystemProperties *ui;
};

#endif // SYSTEMPROPERTIES_H
