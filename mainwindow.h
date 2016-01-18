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

signals:
    void updateSys();

public slots:
    void saveParticles();

    void loadParticles(QString filename="");

    void toggleAutoCoff(bool ok); //переключает коэффиценты из ручного решима в автоматический

    void scaleUp(); //Увеличивает масштаб области отображения
    void scaleDown(); //уменьшает масштаб области отображения

    void recalcSystemInfo(); //пересчитывает параметры на всех формах

    //операции с минимумом и максимумом
    void setMinState();
    void getMinState();
    void setMaxState();
    void getMaxState();
    void clearCurrentState();

private:
    Ui::MainWindow *ui;
    SystemProperties sysprop;
    PartArray sys;
};

#endif // MAINWINDOW_H
