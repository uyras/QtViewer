#include "magneticcircle.h"

MagneticCircle::MagneticCircle(float size, QObject *parent)
{
    this->size = size;
}

QRectF MagneticCircle::boundingRect() const
{
    qreal mLen = (sqrt(pow(M().x(), 2) + pow(M().y(), 2)))*size;
    return QRectF(mLen*-1.,mLen*-1.,mLen*2.,mLen*2.);
}

void MagneticCircle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    qreal mLen = (sqrt(pow(M().x(), 2) + pow(M().y(), 2)))*size;
    painter->drawEllipse(mLen*-1.,mLen*-1.,mLen*2.,mLen*2.);
    painter->drawLine(QPointF(0,0),M()*size);
}

QPointF MagneticCircle::M() const
{
    return m;
}

void MagneticCircle::setM(const QPointF &value)
{
    m = value;
}

