#include "partgraphicsitem.h"
#include"mygraphicsscene.h"
#include <cmath>

PartGraphicsItem::PartGraphicsItem(Part *p):
    part(p)
{
    setFlag(ItemIsSelectable);
}

QRectF PartGraphicsItem::boundingRect() const{
    double mlen = part->m.length() * myScene()->mCoff;
    return QRectF(-1. * mlen, -1. * mlen, 2. * mlen, 2. * mlen);
}

QPainterPath PartGraphicsItem::shape() const{
    QPainterPath path;
    double r = part->m.length() * myScene()->mCoff;
    path.addEllipse(r*-1, r*-1, r*2+1, r*2+1);
    return path;
}

void PartGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget){
    Q_UNUSED(widget);
    Q_UNUSED(item);

    /*#ifdef QT_DEBUG //рисовать окантовки
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(boundingRect());
    painter->drawPath(shape());
    #endif*/

    painter->rotate(part->m.grade());
    double mlen = part->m.length() * myScene()->mCoff;

    //маленький красный крестик в центре частицы
    painter->setPen(Qt::red);
    painter->drawLine(-0.1 * mlen, 0.1 * mlen, 0.1 * mlen, -0.1* mlen);
    painter->drawLine(0.1 * mlen, 0.1 * mlen, -0.1 * mlen, -0.1 * mlen);

    painter->setPen(Qt::black);
    //магнитный момент
    if (part->m.x!=0 || part->m.y!=0){
        if (myScene()->doubleArrows)
            painter->drawLine(-1. * mlen, 0, mlen*0.7,0);
        else
            painter->drawLine(0, 0, mlen*0.7, 0);
    }

    //наконечник стрелки магнитного момента
    if (part->state)
        painter->setBrush(QBrush(Qt::red));
    else
        painter->setBrush(QBrush(Qt::black));

    const QPointF points[3] = {
        QPointF(mlen * 0.7, -0.2 * mlen),
        QPointF(mlen * 0.7, 0.2 * mlen),
        QPointF(mlen, 0)
    };
    painter->drawPolygon(points,3);

    //Id частицы
    if (myScene()->showId)
        painter->drawText(0,0,QString::number(part->Id()));

}

int PartGraphicsItem::type() const
{
    return Type;
}

Part* PartGraphicsItem::PartGraphicsItem::P()
{
    return part;
}

MyGraphicsScene* PartGraphicsItem::myScene() const
{
    return qobject_cast<MyGraphicsScene*>(scene());
}

void PartGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug()<<"clicked on part";
    this->part->rotate();
    emit myScene()->systemChanged();
    this->update();
}

void PartGraphicsItem::updatePos()
{
    this->setPos(part->pos.x * myScene()->spaceCoff, part->pos.y * myScene()->spaceCoff);
}

