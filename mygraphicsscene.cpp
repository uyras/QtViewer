#include "mygraphicsscene.h"

MyGraphicsScene::MyGraphicsScene(QObject *parent) :
    QGraphicsScene(parent),
    doubleArrows(true),
    circle(NULL),
    spaceCoff(1),
    mCoff(1),
    backupSys(0)
{
    Q_UNUSED(parent);
    setItemIndexMethod(NoIndex);
    sys = new PartArray();
    backupSys = new PartArray();
    connect(this,SIGNAL(changed(QList<QRectF>)),this,SIGNAL(systemChanged()));
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
    backupSys->load(file);
    *sys = *backupSys;
    sys->state = backupSys->state;
    //рисуем частицы
    vector<Part*>::iterator iter = sys->parts.begin();
    while (iter != sys->parts.end()){
        emit this->attach(*iter);
        iter++;
    }
    setAutoCoffs();
    emit update();
    emit systemChanged();
}

void MyGraphicsScene::save(QString file)
{
    backupSys->state = sys->state;
    backupSys->setMaxstate(sys->Maxstate());
    backupSys->setMinstate(sys->Minstate());
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

void MyGraphicsScene::setAutoCoffs()
{
    const double normalM = 25.,
            normalSpace=50.;
    double averM=0., minSpace=sys->parts[0]->pos.space(sys->parts[1]->pos);
    Part *temp1, *temp2;
    vector<Part*>::iterator iter1 = sys->parts.begin(), iter2;
    int i=0;
    while (iter1!=sys->parts.end()){
        temp1 = *iter1;
        averM = (averM*(double)i+temp1->m.length())/(double)(i+1);
        iter2=iter1;
        iter2++;
        while (iter2!=sys->parts.end()){
            temp2 = *iter2;
            temp1->pos.space(temp2->pos);
            iter2++;
        }
        iter1++; i++;
    }
    setSpaceCoff(normalSpace/minSpace);
    setMCoff(normalM/averM);
    update();
}

void MyGraphicsScene::keyPressEvent(QKeyEvent *event){

}
