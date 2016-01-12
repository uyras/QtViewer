#ifndef MYGRAPHICSSCENE_H
#define MYGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QKeyEvent>
#include <QtWidgets>
#include <vector>
#include "PartArray.h"
#include "partgraphicsitem.h"
#include "magneticcircle.h"

class PartGraphicsItem;
class MagneticCircle;

class MyGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MyGraphicsScene(QObject *parent = 0);
    virtual ~MyGraphicsScene();

    QPointF startDot, endDot;
    int E1;

    float getSize() const;
    void setSize(float value);
    void attach(Part *part);
    bool doubleArrows, autoScale, ruler;
    void setSpaceCoff(double coff); //установить пространственный коэфф. для системы
    void setMCoff(double coff); //установитькоэфф. магнитного момента
    PartArray* sys;

signals:
    void setM(double);
    void setE1(double);
    void setE2(double);

public slots:

    void clear();
    void load(QString file);
    void save(QString file);
    void addMagneticCircle(); //нарисовать магнитный круг вокруг выделенных частиц
    void removeMagneticCircle(); //удалить магнитный круг с рисунка
    void updateMagneticCirclePos(); //обновить позицию магнитного круга


private:
    PartArray * backupSys;
    MagneticCircle* circle;
    float _size;
    double spaceCoff, mCoff;
    void keyPressEvent(QKeyEvent *event);
};

#endif // MYGRAPHICSSCENE_H
