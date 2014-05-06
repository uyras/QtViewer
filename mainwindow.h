#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "generatedialog.h"
#include "PartArray.h"
#include "Part.h"
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
    PartArray* Parts;


public slots:

    //генерим частицы
    void generate();

	//пересчитать и отобразить поля взаимодействия
	void calcH();

    void initParts();

    void on_surface_destroyed();

    void paintEvent( QPaintEvent *event );

    void on_stateNum_valueChanged(unsigned long long int arg1);

    void on_appendState_clicked();

	void on_pushButton_2_clicked();

	void on_genBtn_clicked();

    void on_pushButton_4_clicked();

    void saveParticles();

    void loadParticles();

    void clearParticles();

    void setMState(double m);
    void setE1State(double e1);
    void setE2State(double e2);

	void recalcParameters(); //пересчитать параметры системы
	void recalcParameters(PartArray*); //пересчитать параметры системы (перегрузка для сигналов)

    void scaleSystem(); //масштабировать систему (не меняя диаметра частиц)

    void scaleUp(); //Увеличивает масштаб области отображения
    void scaleDown(); //уменьшает масштаб области отображения

signals:
	void drawParts(PartArray*); //нарисовать частицы
	void reDrawParts(PartArray*); //перерисовать частицы

private slots:
	void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    generateDialog* gd;
    void emptyENFolder();

    //statusBarLabels
    QLabel
        *mState,
        *e1State,
        *e2State;
};

#endif // MAINWINDOW_H
