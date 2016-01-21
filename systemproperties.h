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
    void changeInteractionRange();

private:
    Ui::SystemProperties *ui;
    QWidget *p;
    QString split(const QString &s);
};

#endif // SYSTEMPROPERTIES_H
