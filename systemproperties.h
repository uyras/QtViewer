#ifndef SYSTEMPROPERTIES_H
#define SYSTEMPROPERTIES_H

#include <QDockWidget>
#include "PartArray.h"

namespace Ui {
class SystemProperties;
}

class SystemProperties : public QDockWidget
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
