#ifndef PARTGRAPHICSITEM_H
#define PARTGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>

#include <Part.h>
#include <config.h>
#include <Vect.h>
#include <mygraphicsscene.h>

class MyGraphicsScene;

class PartGraphicsItem : public QGraphicsItem
{
public:
    PartGraphicsItem(Part* p);

    enum { Type = UserType + 1 };

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);
    int type() const;

    void setPos(const qreal x, const qreal y);
    void setRealPos(const qreal x, const qreal y);
    void setM(Vect m);
    void setH(Vect h);
    void setRadius(const double r);
    void setBrush(const QBrush &b);

    QPointF Pos();
    Vect &M() const;
    Vect &H() const;
    Vect &realPos() const;
    double Radius() const;
    MyGraphicsScene* scene() const;

    Part *getPart() const;
    void setPart(Part *value);

private:
    double radius;
    QBrush ellipseBrush;
    Part* part;
};

#endif // PARTGRAPHICSITEM_H
