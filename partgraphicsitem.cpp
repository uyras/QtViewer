#include "partgraphicsitem.h"
#include <cmath>

PartGraphicsItem::PartGraphicsItem(double radius, Vect m, Vect h)
{
    setRadius(radius);
    setM(m);
    setH(h);
    this->setFlags(PartGraphicsItem::ItemIsSelectable | PartGraphicsItem::ItemIsMovable);
    ellipseBrush.setColor(Qt::green);
}

QRectF PartGraphicsItem::boundingRect() const{
    double coff = (scene())->getScale();
    double ml = sqrt(M().x* M().x*coff*coff + M().y* M().y*coff*coff),
            hl = sqrt(H().x* H().x*coff*coff + H().y* H().y*coff*coff);
	double max = qMax<double>(Radius(), qMax<double>(ml,hl));
	return QRectF(-1*max, -1*max, 2*max, 2*max);
}

QPainterPath PartGraphicsItem::shape() const{
    QPainterPath path;
	path.addEllipse(QPointF(0,0),Radius(),Radius());
    return path;
}

void PartGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget){
    Q_UNUSED(widget);
    Q_UNUSED(item);
    QColor col;
    double coff = (scene())->getScale();

    if (this->h.length()>0 && this->h.scalar(this->m)<0){
        col = QColor(240,190,15);
	} else
        if (this->state)
            col = QColor(255,145,145);
		else
            col = ellipseBrush.color();

    if (this->isSelected())
        col = col.darker(50);

    painter->setBrush(QBrush(col));

	painter->drawEllipse(QPointF(0,0),Radius(),Radius());

    QPen oldPen = painter->pen();

    //магнитный момент
    if (M().length()>0){
        painter->setPen(Qt::red);
        painter->drawLine(0,0,M().x*coff,M().y*coff);
    }

    //поле взаимодействия
    if (H().length()>0){
        painter->setPen(Qt::blue);
        painter->drawLine(0,0,H().x*coff,H().y*coff);
    }

    painter->setPen(oldPen);
}

int PartGraphicsItem::type() const
{
      return Type;
}


void PartGraphicsItem::setBrush(const QBrush &b){
    this->ellipseBrush = b;
    update();
}

void PartGraphicsItem::setPos(const qreal x, const qreal y){
    double coff = (scene())->getScale();
    this->pos.x = x; this->pos.y = y;
    QPointF pos2 = QPointF(x*coff,y*coff);
    QGraphicsItem::setPos(pos2);
    update();
}

void PartGraphicsItem::setM(Vect m){
    this->m = m;
    update();
}

void PartGraphicsItem::setH(Vect h){
    this->h = h;
    update();
}

void PartGraphicsItem::setRadius(const double r){
    this->radius = r;
    update();
}

Vect PartGraphicsItem::M() const{
    return this->m;
}

Vect PartGraphicsItem::H() const{
    return this->h;
}

double PartGraphicsItem::Radius() const{
    double coff = (scene())->getScale();
    return config::Instance()->partR*coff;
    //return this->radius*multiplier;
}

MyGraphicsScene* PartGraphicsItem::scene() const
{
    return qobject_cast<MyGraphicsScene*>(QGraphicsItem::scene());
}
