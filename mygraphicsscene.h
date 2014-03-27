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
    QGraphicsLineItem *pLine = NULL;

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

signals:
    void setM(double);
    void setE1(double);
    void setE2(double);

public slots:
    //очищает указатели мыши
    void clearMousePointers();
    void fullReDraw(PartArray*);
    //обновляет
    void reDraw(PartArray*);
};

#endif // MYGRAPHICSSCENE_H
