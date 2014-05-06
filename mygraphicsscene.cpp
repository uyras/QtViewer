#include "mygraphicsscene.h"

MyGraphicsScene::MyGraphicsScene(QObject *parent) :
    QGraphicsScene(parent),
    pressed(false)
{
    Q_UNUSED(parent);
}

void MyGraphicsScene::fullReDraw(PartArray* parts){
    //чистим плоскость
    this->clear();
    this->setSceneRect(0,0,parts->size.x*20,parts->size.y*20);



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
