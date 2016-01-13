#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "PartArray.h"
#include "Part.h"
#include "honeycombspinicearray.h"
#include "squarespinicearray.h"
#include "squarelattice.h"
#include <vector>
#include "mygraphicsview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    MyGraphicsScene* Parts;


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
    void emptyENFolder();

    //statusBarLabels
    QLabel
        *mState,
        *e1State,
        *e2State;
};

#endif // MAINWINDOW_H
