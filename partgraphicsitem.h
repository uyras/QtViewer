#ifndef PARTGRAPHICSITEM_H
#define PARTGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>

#include "Part.h"
#include "Vect.h"

class MyGraphicsScene;

class PartGraphicsItem : public QGraphicsItem
{
public:
    PartGraphicsItem(Part* p);

    enum { Type = UserType + 1 };

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) Q_DECL_OVERRIDE;
    int type() const;
    void mCoff(double coff);
    void spaceCoff(double coff);

    Part* P();

    MyGraphicsScene* myScene() const;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) Q_DECL_OVERRIDE;

public slots:
    void updatePos();

private:
    Part* part;
    QRectF boundingRectChache;
};

#endif // PARTGRAPHICSITEM_H
