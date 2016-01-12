#include "mygraphicsscene.h"

MyGraphicsScene::MyGraphicsScene(QObject *parent) :
    QGraphicsScene(parent),
    circle(NULL),
    spaceCoff(0),
    mCoff(0),
    backupSys(0)
{
    Q_UNUSED(parent);
    setItemIndexMethod(NoIndex);
    sys = new PartArray();

    setMCoff(10);
    setSpaceCoff(3);
}

MyGraphicsScene::~MyGraphicsScene()
{
    if (backupSys)
        delete backupSys;
    delete sys;
}

float MyGraphicsScene::getSize() const
{
    return this->_size;
}

void MyGraphicsScene::setSize(float value)
{
    this->_size = value;
}

void MyGraphicsScene::attach(Part *part)
{
    PartGraphicsItem *temp = new PartGraphicsItem(part);
    temp->mCoff(this->mCoff);
    temp->spaceCoff(this->spaceCoff);
    this->addItem(temp);
}

void MyGraphicsScene::setSpaceCoff(double coff)
{
    QList<QGraphicsItem*> ilist = this->items();
    PartGraphicsItem *temp;

    for (int i=0; i<ilist.size(); i++){
        temp = qgraphicsitem_cast<PartGraphicsItem*>(ilist.at(i));
        temp->spaceCoff(coff / this->spaceCoff);
    }
    spaceCoff = coff;
}

void MyGraphicsScene::setMCoff(double coff)
{
    QList<QGraphicsItem*> ilist = this->items();
    PartGraphicsItem *temp;

    for (int i=0; i<ilist.size(); i++){
        temp = qgraphicsitem_cast<PartGraphicsItem*>(ilist.at(i));
        temp->mCoff(coff / this->mCoff);
    }
    mCoff = coff;
}

void MyGraphicsScene::clear()
{
    removeMagneticCircle();
    QGraphicsScene::clear();
    sys->clear();
    spaceCoff = 0; mCoff = 0;
}

void MyGraphicsScene::load(QString file)
{
    backupSys = SysLoader::load(file);
    *sys = *backupSys;
    sys->state = backupSys->state;
    //рисуем частицы
    vector<Part*>::iterator iter = sys->parts.begin();
    while (iter != sys->parts.end()){
        emit this->attach(*iter);
        iter++;
    }
    emit update();
}

void MyGraphicsScene::save(QString file)
{
    backupSys->state = sys->state;
    backupSys->save(file);
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

void MyGraphicsScene::keyPressEvent(QKeyEvent *event){

}
