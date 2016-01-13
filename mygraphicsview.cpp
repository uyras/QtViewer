#include "mygraphicsview.h"

MyGraphicsView::MyGraphicsView(QObject *parent) :
    QGraphicsView(),
    preview(this)
{
    Q_UNUSED(parent);
    _scene = new MyGraphicsScene(this);
    this->setScene(_scene);
    emit changeOperateMode(0);
    preview.setScene(_scene);

    preview.setFixedSize(120,120);
    QGraphicsOpacityEffect *e = new QGraphicsOpacityEffect();
    e->setOpacity(0.3);
    preview.setGraphicsEffect(e);
    preview.setAutoFillBackground(true);
    preview.setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    preview.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    preview.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    preview.setInteractive(false);
    preview.hide();
    connect(_scene,SIGNAL(changed(QList<QRectF>)),this,SLOT(updatePreview(QList<QRectF>)));


    /*
    Part* p = new Part();
    p->m = Vect(10,10,0);
    _scene->attach(p);
    */
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

void MyGraphicsView::changeOperateMode(int mode)
{
    switch(mode){
    case 0: default:
        this->setDragMode(QGraphicsView::ScrollHandDrag);
        this->setInteractive(false);
        break;
    case 1:
        this->setDragMode(QGraphicsView::RubberBandDrag);
        this->setInteractive(true);
        break;
    }

}

void MyGraphicsView::dbgSlot()
{
    scene()->sys->save("1.dat");
    scene()->update();
}

void MyGraphicsView::toggleDoubleArrows(bool on)
{
    scene()->doubleArrows = on;
    emit scene()->update();
}

void MyGraphicsView::keyPressEvent(QKeyEvent *event){
    qDebug()<<this->geometry().width();
    if (event->modifiers() & (Qt::ShiftModifier | Qt::AltModifier)){
        event->accept();
    }
    QGraphicsView::keyPressEvent(event);
}

void MyGraphicsView::keyReleaseEvent(QKeyEvent *event){
    //switch()
    if ( !(event->modifiers()&(Qt::ShiftModifier | Qt::AltModifier))){
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

void MyGraphicsView::resizeEvent(QResizeEvent *event)
{
    preview.move(event->size().width()-preview.width(),0);
}

void MyGraphicsView::updatePreview(const QList<QRectF> &regions)
{
    preview.setSceneRect(scene()->sceneRect());
    qDebug()<<Random::Instance()->next()<<"called changed with regions"<<regions.size();
    qDebug()<<regions;
}

/*void MyGraphicsView::drawForeground(QPainter *paint, const QRectF &rect)
{
    paint->drawLine(0,0,20,20);
}*/
