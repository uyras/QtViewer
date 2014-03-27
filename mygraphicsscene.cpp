#include "mygraphicsscene.h"

MyGraphicsScene::MyGraphicsScene(QObject *parent) :
    QGraphicsScene(parent),
    pLine(new QGraphicsLineItem())
{
    Q_UNUSED(parent);
}

void MyGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    this->endDot = this->startDot = event->scenePos();

    if (!this->items().contains(pLine)){
        this->addItem(pLine);
    }
    pLine->setLine(QLineF(startDot,endDot));

    //делаем все точки белыми
    this->clearSelection();/*
    QList<QGraphicsItem *> l = this->items();
    PartGraphicsItem *temp;
    for (int i=0;i<l.size();i++){
        temp = qgraphicsitem_cast<PartGraphicsItem *>(l[i]);
        if (temp!=0)
            temp->setBrush(Qt::white);
    }*/
}

void MyGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    this->endDot = event->scenePos();

    //подсвечиваем выделенные точки красными
    QList<QGraphicsItem *> l1,l2,l;
    PartGraphicsItem *temp, *startT=0, *endT=0;
    if (startDot==endDot){
        QPointF click = startDot;
        emit clearMousePointers();
        temp = qgraphicsitem_cast<PartGraphicsItem *>(this->itemAt((qreal)click.x(),(qreal)click.y(),QTransform()));
        if (temp !=0){
            Part *tempP = temp->P();
            l1 =	this->items();
            for (int i=0;i<l1.size();i++){
                temp = qgraphicsitem_cast<PartGraphicsItem *>(l1[i]);
                if (temp!=0 && temp->P() != tempP){
                    Vect h1 = temp->P()->interact(tempP);
                    temp->setH(QPointF(h1.x,h1.y));
                }
            }
        }

    } else {
        this->clearSelection();
        temp = qgraphicsitem_cast<PartGraphicsItem *>(this->itemAt(startDot.rx(), startDot.ry(), QTransform()));
        if (temp!=NULL)
            temp->setSelected(true);

        temp = qgraphicsitem_cast<PartGraphicsItem *>(this->itemAt(endDot.rx(), endDot.ry(), QTransform()));
        if (temp!=NULL)
            temp->setSelected(true);

/*
        for (int i=0;i<l1.size();i++){
            temp = qgraphicsitem_cast<PartGraphicsItem *>(l1[i]);
            if (temp!=0) {
                //temp->setBrush(Qt::yellow);
                startT = temp;
            }
        }
        for (int i=0;i<l2.size();i++){
            temp = qgraphicsitem_cast<PartGraphicsItem *>(l2[i]);
            if (temp!=0) {
                //temp->setBrush(Qt::yellow);
                endT = temp;
            }
        }
        if (startT!=0 && endT!=0){
            Vect h = endT->P()->interact(startT->P());
            emit setE1(h.scalar(startT->P()->m));
        }*/
    }


    //получаем связанные частицы и считае
}

void MyGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    this->endDot = event->scenePos();
    pLine->setLine(QLineF(startDot,endDot));
}

void MyGraphicsScene::clearMousePointers(){
    startDot = endDot = QPointF(0,0);
    pLine->setLine(QLineF(startDot,endDot));
    this->removeItem(pLine);
}

void MyGraphicsScene::fullReDraw(PartArray* parts){
    //чистим плоскость
    this->clear();
    //восстанавливаем линию
    pLine = new QGraphicsLineItem();
    this->addItem(pLine);



    //рисуем частицы
    vector<Part*>::iterator
            begin = parts->parts.begin(),
            end = parts->parts.end(),
            iter = begin;

    PartGraphicsItem *temp;

    int i=0;
    while (iter != end){
        temp = new PartGraphicsItem(
                    0.5,
                    (*iter),
                    QPointF((*iter)->m.x,(*iter)->m.y),
                    QPointF((*iter)->h.x,(*iter)->h.y)
                    );
        temp->setPos((*iter)->pos.x,(*iter)->pos.y);
        this->addItem(temp);

        iter++;
        i++;
    }
}


void MyGraphicsScene::reDraw(PartArray* parts){
    Q_UNUSED(parts);
    QList<QGraphicsItem *> l;
    PartGraphicsItem *temp;
    l = this->items();
    for (int i=0;i<l.size();i++){
        temp = qgraphicsitem_cast<PartGraphicsItem *>(l[i]);
        if (temp!=0){
            temp->setM(QPointF(temp->P()->m.x,temp->P()->m.y));
            if (temp->P()->h.length()!=0)
                temp->setH(QPointF(temp->P()->h.x,temp->P()->h.y));
        }
    }
    emit this->update();
}
