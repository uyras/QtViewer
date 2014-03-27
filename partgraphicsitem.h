#ifndef PARTGRAPHICSITEM_H
#define PARTGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>

#include <Part.h>
#include <config.h>

class PartGraphicsItem : public QGraphicsItem
{
public:
    PartGraphicsItem(double radius, Part *p, QPointF m = QPointF(), QPointF h = QPointF());

    enum { Type = UserType + 1 };

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);
    int type() const;

    void setPos(const qreal x, const qreal y);
    void setM(const QPointF &m);
    void setH(const QPointF &h);
    void setRadius(const double r);
    void setBrush(const QBrush &b);
    void setPart(Part *p);

    //QPointF Pos();
    QPointF M() const;
    QPointF H() const;
    double Radius() const;
    Part* P() const;

private:
    QPointF pos,m,h;
    qint8 multiplier; //множитель, на который домножаются параметры при отрисовке
    double radius;
    QBrush ellipseBrush;
    Part *p;
};

#endif // PARTGRAPHICSITEM_H
