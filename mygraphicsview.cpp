#include "mygraphicsview.h"

MyGraphicsView::MyGraphicsView(QObject *parent) :
    QGraphicsView()
{
    Q_UNUSED(parent);
    _scene = new MyGraphicsScene(this);
    this->setScene(_scene);
}



MyGraphicsScene *MyGraphicsView::scene(){
    return _scene;
}

void MyGraphicsView::scaleTo(int size){
    double size2 = qreal(size)/50.;
    QMatrix matrix;
    matrix.scale(size2, size2);
    setMatrix(matrix);
}

void MyGraphicsView::keyPressEvent(QKeyEvent *event){
    if (event->modifiers() & (Qt::ShiftModifier | Qt::AltModifier)){
        this->setDragMode(QGraphicsView::ScrollHandDrag);
        this->setInteractive(false);
        event->accept();
    }
    QGraphicsView::keyPressEvent(event);
}

void MyGraphicsView::keyReleaseEvent(QKeyEvent *event){
    if ( !(event->modifiers()&(Qt::ShiftModifier | Qt::AltModifier))){
        this->setDragMode(QGraphicsView::RubberBandDrag);
        this->setInteractive(true);
        event->accept();
    }
    QGraphicsView::keyPressEvent(event);
}

void MyGraphicsView::wheelEvent(QWheelEvent *event){

    if (!event->modifiers()) {
        if (event->delta() > 0)
            emit scaleUp();
        else
            emit scaleDown();
        event->accept();
    } else if (event->modifiers() == Qt::ControlModifier){
        this->horizontalScrollBar()->setValue(this->horizontalScrollBar()->value()-event->delta());
    } else if (event->modifiers() == Qt::AltModifier){
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value()-event->delta());
    }
    QGraphicsView::wheelEvent(event);
}
