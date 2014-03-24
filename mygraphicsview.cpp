#include "mygraphicsview.h"

MyGraphicsView::MyGraphicsView(QObject *parent) :
    QGraphicsScene(),
    pLine(new QGraphicsLineItem())
{
    Q_UNUSED(parent);
}

void MyGraphicsView::mousePressEvent(QMouseEvent *event){
    this->endDot = this->startDot = event->pos();

    if (!this->items().contains(pLine)){
        this->addItem(pLine);
    }

    //делаем все точки белыми
    QList<QGraphicsItem *> l = this->items();
    PartGraphicsItem *temp;
	for (int i=0;i<l.size();i++){
        temp = qgraphicsitem_cast<PartGraphicsItem *>(l[i]);
		if (temp!=0)
			temp->setBrush(Qt::white);
	}
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent *event){
    this->endDot = event->pos();
    pLine->setLine(QLine(startDot,endDot));

    //подсвечиваем выделенные точки красными
	QList<QGraphicsItem *> l1,l2,l;
	PartGraphicsItem *temp, *startT=0, *endT=0;
    if (startDot==endDot){
        this->removeItem(pLine);

        temp = qgraphicsitem_cast<PartGraphicsItem *>(this->itemAt((qreal)startDot.x(),(qreal)startDot.y(),QTransform()));
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
        l1 = this->items(QPointF(startDot.x(),startDot.y()));
        l2 = this->items(QPointF(endDot.x(),endDot.y()));

		for (int i=0;i<l1.size();i++){
			temp = qgraphicsitem_cast<PartGraphicsItem *>(l1[i]);
			if (temp!=0) {
                temp->setBrush(Qt::yellow);
				startT = temp;
			}
		}
		for (int i=0;i<l2.size();i++){
			temp = qgraphicsitem_cast<PartGraphicsItem *>(l2[i]);
			if (temp!=0) {
				temp->setBrush(Qt::yellow);
				endT = temp;
			}
		}
		if (startT!=0 && endT!=0){
			Vect h = endT->P()->interact(startT->P());
			emit setE1(h.scalar(startT->P()->m));
		}
	}

	//получаем связанные частицы и считае
}

void MyGraphicsView::mouseMoveEvent(QMouseEvent *event){
        this->endDot = event->pos();
        pLine->setLine(QLine(startDot,endDot));
}

void MyGraphicsView::clearMousePointers(){
    startDot.setX(0);
    startDot.setY(0);
    endDot.setX(0);
    endDot.setY(0);
}

void MyGraphicsView::reDraw(PartArray* parts){
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

void MyGraphicsView::fullReDraw(PartArray* parts){
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

void MyGraphicsView::scaleTo(int size){
    double size2 = (qreal(size))/50.;
    QMatrix matrix;
    matrix.scale(size2, size2);
    //setMatrix(matrix);

}
