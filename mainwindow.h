#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <vector>
#include "PartArray.h"
#include "mygraphicsview.h"
#include "systemproperties.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:

    void paintEvent( QPaintEvent *event );

    void saveParticles();

    void loadParticles(QString filename="");

    void clearParticles();

    void setMState(double m);
    void setE1State(double e1);
    void setE2State(double e2);

    void scaleSystem(); //масштабировать систему (не меняя диаметра частиц)

    void scaleUp(); //Увеличивает масштаб области отображения
    void scaleDown(); //уменьшает масштаб области отображения

    void recalcSystemValues();

private:
    Ui::MainWindow *ui;
    SystemProperties sysprop;
    PartArray sys;
    void emptyENFolder();
};

#endif // MAINWINDOW_H
