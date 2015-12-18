#include "partgraphicsitem.h"
#include <cmath>

PartGraphicsItem::PartGraphicsItem(Part *p, float size):
    part(NULL)
{
    setAcceptDrops(true);
    setSize(size);
    setPart(p);
    this->setFlags(PartGraphicsItem::ItemIsSelectable | PartGraphicsItem::ItemIsMovable | ItemSendsGeometryChanges);
    ellipseBrush.setColor(Qt::green);
}

QRectF PartGraphicsItem::boundingRect() const{
    return boundingRectChache;
}

void PartGraphicsItem::setBoundingRect(){
    if (this->getPart()){
        double coff = this->getSize();
        double r = Radius();
        double mx = std::max<double>(std::fabs(M().x),std::fabs(H().x))*coff;
        mx = (r>mx)?r:mx;
        double my = std::max<double>(std::fabs(M().y),std::fabs(H().y))*coff;
        my = (r>my)?r:my;
        boundingRectChache = QRectF(-1*mx, -1*my, 2*mx, 2*my);
    }
}

QPainterPath PartGraphicsItem::shape() const{
    QPainterPath path;
    double r = Radius();
    path.addEllipse(r*-1,r*-1,r*2,r*2);
    return path;
}

void PartGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget){
    Q_UNUSED(widget);
    Q_UNUSED(item);
    QColor col = ellipseBrush.color();
    double coff = getSize();

    if (this->isSelected())
        col = col.darker(50);

    painter->setBrush(QBrush(col));

    double r = Radius();
    painter->drawEllipse(r*-1,r*-1,r*2,r*2);

    QPen oldPen = painter->pen();

    //магнитный момент
    if (M().x!=0 || M().y!=0){
        painter->setPen(Qt::red);
        painter->drawLine(0,0,M().x*coff,M().y*coff);
    }

    //поле взаимодействия
    if (H().x!=0 || H().y!=0){
        painter->setPen(Qt::blue);
        painter->drawLine(0,0,H().x*coff,H().y*coff);
    }

    painter->setPen(oldPen);
    if (false){
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect());
    }
}

int PartGraphicsItem::type() const
{
      return Type;
}


void PartGraphicsItem::setBrush(const QBrush &b){
    this->ellipseBrush = b;
    update();
}

void PartGraphicsItem::setSize(float size)
{
    this->_size = size;
    emit setBoundingRect();
    if (this->getPart())
        setPos(this->getPart()->pos.x*size, this->getPart()->pos.y*size);
}

void PartGraphicsItem::setRealPos(const qreal x, const qreal y)
{
    setPos(x*getSize(),y*getSize());
}

void PartGraphicsItem::setM(Vect m){
    this->getPart()->m = m;
    update();
    emit setBoundingRect();
}

void PartGraphicsItem::setH(Vect h){
    this->getPart()->h = h;
    emit setBoundingRect();
    update();
}

void PartGraphicsItem::setRadius(const double r){
    this->radius = r;
    emit setBoundingRect();
    update();
}

Vect& PartGraphicsItem::M() const{
    return this->getPart()->m;
}

Vect& PartGraphicsItem::H() const{
    return this->getPart()->h;
}

QPointF PartGraphicsItem::m() const
{
    return QPointF(this->getPart()->m.x, this->getPart()->m.y);
}

Vect &PartGraphicsItem::realPos() const
{
    return this->getPart()->pos;
}

double PartGraphicsItem::Radius() const{
    return config::Instance()->partR*getSize();
    //return this->radius*multiplier;
}

MyGraphicsScene* PartGraphicsItem::scene() const
{
    return qobject_cast<MyGraphicsScene*>(QGraphicsItem::scene());
}

Part *PartGraphicsItem::getPart() const
{
    return part;
}

void PartGraphicsItem::setPart(Part *value)
{
    part = value;
    emit setBoundingRect();
    setPos(value->pos.x*getSize(), value->pos.y*getSize());
}

float PartGraphicsItem::getSize() const
{
    return _size;
}

QVariant PartGraphicsItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionHasChanged){
        QPointF newPos = value.toPointF();
        Part* p = getPart();
        p->pos.x = newPos.x()/getSize();
        p->pos.y = newPos.y()/getSize();
    }
    if (change == ItemSelectedHasChanged){
        this->setZValue(value.toInt());
    }
    return QGraphicsItem::itemChange(change, value);
}

