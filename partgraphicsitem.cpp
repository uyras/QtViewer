#include "partgraphicsitem.h"
#include <cmath>

PartGraphicsItem::PartGraphicsItem(double radius, Part *p, QPointF m, QPointF h):
    multiplier(20)
{
    setRadius(radius);
    setM(m);
    setH(h);
    setPart(p);
    this->setFlags(PartGraphicsItem::ItemIsSelectable | PartGraphicsItem::ItemIsMovable);
    ellipseBrush.setColor(Qt::green);
}

QRectF PartGraphicsItem::boundingRect() const{
	double ml = sqrt(M().x()* M().x() + M().y()* M().y()),
			hl = sqrt(H().x()* H().x() + H().y()* H().y());
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

    if (P()->h.length()>0 && P()->h.scalar(P()->m)<0){
        col = QColor(240,190,15);
	} else
        if (P()->state)
            col = QColor(255,145,145);
		else
            col = ellipseBrush.color();

    if (this->isSelected())
        col = col.darker(50);

    painter->setBrush(QBrush(col));

	painter->drawEllipse(QPointF(0,0),Radius(),Radius());

    QPen oldPen = painter->pen();

    //магнитный момент
    if (!M().isNull()){
        painter->setPen(Qt::red);
        painter->drawLine(QPointF(0,0),M());
    }

    //поле взаимодействия
    if (!H().isNull()){
        painter->setPen(Qt::blue);
        painter->drawLine(QPointF(0,0),H());
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
    QPointF pos2 = QPointF(x*multiplier,y*multiplier);
    QGraphicsItem::setPos(pos2);
    update();
}

void PartGraphicsItem::setM(const QPointF &m){
    this->m = m;
    update();
}

void PartGraphicsItem::setH(const QPointF &h){
    this->h = h;
    update();
}

void PartGraphicsItem::setRadius(const double r){
    this->radius = r;
    update();
}

void PartGraphicsItem::setPart(Part *p){
    this->p = p;
}

QPointF PartGraphicsItem::M() const{
    return this->m*multiplier;
}

QPointF PartGraphicsItem::H() const{
    return this->h*multiplier;
}

double PartGraphicsItem::Radius() const{
    return config::Instance()->partR*multiplier;
    //return this->radius*multiplier;
}

Part* PartGraphicsItem::P() const{
    return this->p;
}
