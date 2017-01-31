#include "mygraphicsscene.h"

MyGraphicsScene::MyGraphicsScene(QObject *parent) :
    QGraphicsScene(parent),
    doubleArrows(true),
    showId(false),
    circle(NULL),
    spaceCoff(1),
    mCoff(1)
{
    Q_UNUSED(parent);
    setItemIndexMethod(NoIndex);
    connect(this,SIGNAL(changed(QList<QRectF>)),this,SIGNAL(systemChanged()));
}

void MyGraphicsScene::init(PartArray *sys)
{
    this->clear();
    //рисуем частицы
    vector<Part*>::iterator iter = sys->parts.begin();
    while (iter != sys->parts.end()){
        PartGraphicsItem *temp = new PartGraphicsItem(*iter);
        this->addItem(temp);
        temp->updatePos();
        iter++;
    }
    emit update();
}

float MyGraphicsScene::getSize() const
{
    return this->_size;
}

void MyGraphicsScene::setSize(float value)
{
    this->_size = value;
}

void MyGraphicsScene::setCoffs(double m, double space)
{
    this->spaceCoff = space;
    this->mCoff = m;

    this->updatePoses();
}

void MyGraphicsScene::updatePoses()
{
    QList<QGraphicsItem*> lst = this->items();
    QList<QGraphicsItem*>::iterator iter = lst.begin();
    while (iter!=lst.end()){
        PartGraphicsItem* temp = qgraphicsitem_cast<PartGraphicsItem*>(*iter);
        emit temp->updatePos();
        iter++;
    }
    update();
}

void MyGraphicsScene::clear()
{
    QGraphicsScene::clear();
    spaceCoff = 1; mCoff = 1;
}

void MyGraphicsScene::addMagneticCircle()
{
    this->circle = new MagneticCircle(this->getSize());
    this->addItem(circle);
    circle->setZValue(2);
    emit updateMagneticCirclePos();
}

void MyGraphicsScene::removeMagneticCircle()
{
    this->removeItem(this->circle);
    delete this->circle;
    this->circle = NULL;
}

void MyGraphicsScene::updateMagneticCirclePos()
{
    QList<QGraphicsItem *> selected = selectedItems();

    if (selected.size()>1 && circle==NULL){ //если элемент не добавлен, добавляем
        addMagneticCircle();
    }
    if (selected.size()<=1 && circle!=NULL){ //если элемент добавлен, удаляем
        removeMagneticCircle();
    }
    if (circle != NULL){
        PartGraphicsItem* item;
        QGraphicsItem* temp;
        QList<QGraphicsItem *>::Iterator iter = selected.begin();
        Vect m(0,0,0); //магнитный момент выделенных частиц
        qreal minx,miny,maxx,maxy;
        while (iter!=selected.end()){
            temp = *iter;
            item = qgraphicsitem_cast<PartGraphicsItem *>(temp);
            if (iter==selected.begin()){
                minx = maxx = item->P()->pos.x;
                miny = maxy = item->P()->pos.y;
            } else {
                minx = qMin<qreal>(item->P()->pos.x,minx);
                miny = qMin<qreal>(item->P()->pos.y,miny);
                maxx = qMax<qreal>(item->P()->pos.x,maxx);
                maxy = qMax<qreal>(item->P()->pos.y,maxy);
            }
            m+=item->P()->m;
            iter++;
        }
        circle->setM(QPointF(m.x, m.y));
        circle->setPos((minx+maxx)/2.,(miny+maxy)/2.);
    }
}
