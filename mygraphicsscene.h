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

    void init(PartArray* sys);

    QPointF startDot, endDot;
    int E1;

    float getSize() const;
    void setSize(float value);
    bool doubleArrows, autoScale, ruler, showId;
    void setCoffs(double m, double space); //установить пространственный коэфф. для системы
    double spaceCoff, mCoff;
    void updatePoses();

signals:
    void systemChanged();

public slots:

    void clear();
    void addMagneticCircle(); //нарисовать магнитный круг вокруг выделенных частиц
    void removeMagneticCircle(); //удалить магнитный круг с рисунка
    void updateMagneticCirclePos(); //обновить позицию магнитного круга

private:
    MagneticCircle* circle;
    float _size;
};

#endif // MYGRAPHICSSCENE_H
