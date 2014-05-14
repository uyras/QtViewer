#ifndef MYGRAPHICSSCENE_H
#define MYGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QKeyEvent>
#include <vector>
#include "PartArray.h"
#include "partgraphicsitem.h"

class PartGraphicsItem;

class MyGraphicsScene : public QGraphicsScene, public PartArray
{
    Q_OBJECT
public:
    explicit MyGraphicsScene(QObject *parent = 0);

    QPointF startDot, endDot;
    int E1;

    std::vector<PartGraphicsItem*> parts;

    float getScale() const;
    void setScale(float value);
    void insert(Part *part);
    void attach(Part *part);

signals:
    void setM(double);
    void setE1(double);
    void setE2(double);

public slots:
    void addParts();
    void fullReDraw();
    //обновляет
    void reDraw();
    void moveUp(float size);
    void moveDown(float size);
    void moveLeft(float size);
    void moveRight(float size);
    void clear();
    void resize(double x, double y);
    void dropRandom(double maxDestiny);


private:
    float scale;
    bool pressed;
    void _addBorders();
    void _addRule();
    void keyPressEvent(QKeyEvent *event);
};

#endif // MYGRAPHICSSCENE_H
