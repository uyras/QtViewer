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
    double size2 = (qreal(size))/50.;
    QMatrix matrix;
    matrix.scale(size2, size2);
    setMatrix(matrix);

}
