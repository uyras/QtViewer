#ifndef MYGRAPHICSSCENE_H
#define MYGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include "PartArray.h"
#include "partgraphicsitem.h"

class MyGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MyGraphicsScene(QObject *parent = 0);

    QPointF startDot, endDot;

signals:
    void setM(double);
    void setE1(double);
    void setE2(double);

public slots:
    void fullReDraw(PartArray*);
    //обновляет
    void reDraw(PartArray*);

private:
    bool pressed;
    void _addBorders();
    void _addRule();
};

#endif // MYGRAPHICSSCENE_H
