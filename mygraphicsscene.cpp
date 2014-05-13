#include "mygraphicsscene.h"

MyGraphicsScene::MyGraphicsScene(QObject *parent) :
    QGraphicsScene(parent),
    pressed(false)
{
    setScale(20.);
    Q_UNUSED(parent);
}

void MyGraphicsScene::fullReDraw(){

    //добавляем окантовку и оси
    this->_addBorders();
    this->_addRule();

    //рисуем частицы
    vector<Part*>::iterator
            begin = PartArray::parts.begin(),
            end = PartArray::parts.end(),
            iter = begin;
    PartGraphicsItem* temp;
    Part* temp2;
    while (iter != end){
        temp2 = *iter;
        temp = reinterpret_cast<PartGraphicsItem*>(temp2);
        this->parts.push_back(temp);
        this->addItem(temp);
        iter++;
    }
}


void MyGraphicsScene::reDraw(){
    emit this->update();
}

void MyGraphicsScene::moveUp(float size=0.1)
{
    QList<QGraphicsItem*> items = selectedItems();
    QList<QGraphicsItem*>::Iterator i;
    PartGraphicsItem *temp;
    for(i=items.begin(); i!=items.end(); i++){
        temp = qgraphicsitem_cast<PartGraphicsItem *>(*i);
        temp->setPos(temp->pos.x+size,temp->pos.y);
    }
}

void MyGraphicsScene::moveDown(float size=0.1)
{

}

void MyGraphicsScene::moveLeft(float size=0.1)
{

}

void MyGraphicsScene::moveRight(float size=0.1)
{

}

float MyGraphicsScene::getScale() const
{
    return scale;
}

void MyGraphicsScene::setScale(float value)
{
    scale = value;
}

void MyGraphicsScene::insert(Part *part)
{
    PartGraphicsItem *temp = dynamic_cast<PartGraphicsItem*>(part);
    this->parts.push_back(temp);
    PartArray::insert(part);
}

void MyGraphicsScene::addParts()
{

}

void MyGraphicsScene::clear()
{
    PartArray::clear();
    QGraphicsScene::clear();
}

void MyGraphicsScene::resize(double x, double y)
{
    PartArray::resize(x,y,1);
    this->setSceneRect(0,0,this->size.x*getScale(),this->size.y*getScale());
}

void MyGraphicsScene::_addBorders(){
    QGraphicsItem* i;
    this->addRect(this->sceneRect(),QPen(Qt::black));
}

void MyGraphicsScene::_addRule(){
    qreal w = this->width();
    qreal h = this->height();
    const int length = 3;
    for (qreal x = 0; x<=w; x+=getScale()){
        this->addLine(x,0,x,length,QPen(Qt::black));
        this->addLine(x,w,x,w-length,QPen(Qt::black));
    }

    for (qreal y = 0; y<=h; y+=getScale()){
        this->addLine(0,y,length,y,QPen(Qt::black));
        this->addLine(h,y,h-length,y,QPen(Qt::black));
    }
}

void MyGraphicsScene::keyPressEvent(QKeyEvent *event){
    switch (event->key()) {
    case Qt::Key_Up:
        emit moveUp();
        break;
    case Qt::Key_Down:
        emit moveDown();
        break;
    case Qt::Key_Left:
        emit moveLeft();
        break;
    case Qt::Key_Right:
        emit moveRight();
        break;
    default:
        break;
    }
}
