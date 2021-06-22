#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QFileDialog>
#include <QPainter>
#include <QMessageBox>
#include <QTableWidget>
#include <QtEvents>
#include <QMainWindow>
#include <QDebug>
#include <QLabel>
#include <QSettings>

#include <iostream>
#include <cstring>
#include <string>
#include <cmath>
#include <fstream>
#include <vector>
#include <map>
#include <ctime>

#include "PartArray.h"
#include "Part.h"
#include "Vect.h"
#include "mygraphicsview.h"
#include "systemproperties.h"
#include "statechangedialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);

signals:
    void updateSys();

public slots:
    void saveAsParticles(QString fname = QString());
    void saveParticles();

    void loadParticles(QString filename="");

    void exportImage();

    void toggleAutoCoff(bool ok); //переключает коэффиценты из ручного решима в автоматический

    void scaleUp(); //Увеличивает масштаб области отображения
    void scaleDown(); //уменьшает масштаб области отображения

    void setFileName(QString fname);

    void recalcSystemInfo(); //пересчитывает параметры на всех формах

    //операции с минимумом и максимумом
    void setMinState();
    void getMinState();
    void setMaxState();
    void getMaxState();
    void clearCurrentState();
    void changeCurrentState();
    void setInteractionRange(double);

    void exampleClicked();

private:
    Ui::MainWindow *ui;
    SystemProperties sysprop;
    PartArray sys;
    QString filename;
    Qt::KeyboardModifiers press; //чтобы запомнить кнопки в момент нажатия
    void initExamples();

private slots:
    void releaseModifiers();
};

#endif // MAINWINDOW_H
