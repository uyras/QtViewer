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
    PartGraphicsItem(Part* p, float size);

    enum { Type = UserType + 1 };

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);
    int type() const;

    void setRealPos(const qreal x, const qreal y);
    void setM(Vect m);
    void setH(Vect h);
    void setRadius(const double r);
    void setBrush(const QBrush &b);
    void setSize(float size);

    Vect &M() const;
    Vect &H() const;
    QPointF m() const;
    Vect &realPos() const;
    double Radius() const;
    MyGraphicsScene* scene() const;

    Part *getPart() const;
    void setPart(Part *value);
    float getSize() const;

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private slots:
    void setBoundingRect();
private:
    double radius;
    QBrush ellipseBrush;
    Part* part;
    float _size;
    QRectF boundingRectChache;
};

#endif // PARTGRAPHICSITEM_H
