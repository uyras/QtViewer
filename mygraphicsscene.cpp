#include "mygraphicsscene.h"

MyGraphicsScene::MyGraphicsScene(QObject *parent) :
    QGraphicsScene(parent),
    circle(NULL),
    _slider(NULL)
{
    setSize(20.);
    Q_UNUSED(parent);
    setItemIndexMethod(NoIndex);
    connect(this,SIGNAL(selectionChanged()),SLOT(updateMagneticCirclePos()));
    connect(this,SIGNAL(selectionChanged()),SLOT(showSlider()));
}

void MyGraphicsScene::fullReDraw(){

    //добавляем окантовку и оси
    this->_addBorders();
    this->_addRule();
}


void MyGraphicsScene::reDraw(){
    emit this->update();
}

void MyGraphicsScene::moveUp(float size=0.05)
{
    QList<QGraphicsItem*> items = selectedItems();
    QList<QGraphicsItem*>::Iterator i;
    PartGraphicsItem *temp;
    for(i=items.begin(); i!=items.end(); i++){
        temp = qgraphicsitem_cast<PartGraphicsItem *>(*i);
        temp->setRealPos(temp->realPos().x,temp->realPos().y-size);
    }
}

void MyGraphicsScene::moveDown(float size=0.05)
{
    QList<QGraphicsItem*> items = selectedItems();
    QList<QGraphicsItem*>::Iterator i;
    PartGraphicsItem *temp;
    for(i=items.begin(); i!=items.end(); i++){
        temp = qgraphicsitem_cast<PartGraphicsItem *>(*i);
        temp->setRealPos(temp->realPos().x,temp->realPos().y+size);
    }
}

void MyGraphicsScene::moveLeft(float size=0.05)
{
    QList<QGraphicsItem*> items = selectedItems();
    QList<QGraphicsItem*>::Iterator i;
    PartGraphicsItem *temp;
    for(i=items.begin(); i!=items.end(); i++){
        temp = qgraphicsitem_cast<PartGraphicsItem *>(*i);
        temp->setRealPos(temp->realPos().x-size,temp->realPos().y);
    }
}

void MyGraphicsScene::moveRight(float size=0.05)
{
    QList<QGraphicsItem*> items = selectedItems();
    QList<QGraphicsItem*>::Iterator i;
    PartGraphicsItem *temp;
    for(i=items.begin(); i!=items.end(); i++){
        temp = qgraphicsitem_cast<PartGraphicsItem *>(*i);
        temp->setRealPos(temp->realPos().x+size,temp->realPos().y);
    }
}

void MyGraphicsScene::setFiValue(int value)
{
    qreal fi = ((qreal)value / 100.)*M_PI/180.;
    QList<QGraphicsItem*> items = selectedItems();
    QList<QGraphicsItem*>::Iterator i;
    PartGraphicsItem *temp;
    for(i=items.begin(); i!=items.end(); i++){
        temp = qgraphicsitem_cast<PartGraphicsItem *>(*i);
        double len = temp->getPart()->m.length();
        temp->setM(Vect(len*qCos(fi),len*qSin(fi),0));
    }
}

void MyGraphicsScene::setMValue(int value)
{
    qreal m = (qreal)value / 100.;
    QList<QGraphicsItem*> items = selectedItems();
    QList<QGraphicsItem*>::Iterator i;
    PartGraphicsItem *temp;
    for(i=items.begin(); i!=items.end(); i++){
        temp = qgraphicsitem_cast<PartGraphicsItem *>(*i);
        Vect vm = temp->M();
        vm.setUnit(); //делаем вектор единичным
        temp->setM(Vect(vm.x*m,vm.y*m,0));
    }
}

void MyGraphicsScene::setRValue(int value)
{

}

float MyGraphicsScene::getSize() const
{
    return this->_size;
}

void MyGraphicsScene::setSize(float value)
{
    this->_size = value;
}

void MyGraphicsScene::insert(Part *part)
{
    PartArray::insert(part);
    emit attach(part);
}

void MyGraphicsScene::attach(Part *part)
{
    PartGraphicsItem *temp = new PartGraphicsItem(part, getSize());
    this->addItem(temp);
}

void MyGraphicsScene::addParts()
{

}

void MyGraphicsScene::clear()
{
    removeMagneticCircle();
    _slider = NULL;
    PartArray::clear();
    QGraphicsScene::clear();
}

void MyGraphicsScene::resize(double x, double y)
{
    PartArray::resize(x,y,1);
    this->setSceneRect(0,0,this->size.x*getSize(),this->size.y*getSize());
}

void MyGraphicsScene::dropRandom(double maxDestiny)
{
    int initCount = parts.size();
    PartArray::dropRandom(maxDestiny);
    //рисуем частицы
    vector<Part*>::iterator iter = PartArray::parts.begin()+initCount;
    while (iter != PartArray::parts.end()){
        emit this->attach(*iter);
        iter++;
    }
}

void MyGraphicsScene::dropRandom(int count)
{
    int initCount = parts.size();
    PartArray::dropRandom(count);
    //рисуем частицы
    vector<Part*>::iterator iter = PartArray::parts.begin()+initCount;
    while (iter != PartArray::parts.end()){
        emit this->attach(*iter);
        iter++;
    }
}

void MyGraphicsScene::dropChain(double distance)
{
    int initCount = parts.size();
    PartArray::dropChain(distance);
    //рисуем частицы
    vector<Part*>::iterator iter = PartArray::parts.begin()+initCount;
    while (iter != PartArray::parts.end()){
        emit this->attach(*iter);
        iter++;
    }
}

void MyGraphicsScene::load(char *file, bool showNotifications)
{
    int initCount = parts.size();
    PartArray::load(file,showNotifications);
    //рисуем частицы
    vector<Part*>::iterator iter = PartArray::parts.begin()+initCount;
    while (iter != PartArray::parts.end()){
        emit this->attach(*iter);
        iter++;
    }
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
        PartGraphicsItem* part;
        QGraphicsItem* temp;
        QList<QGraphicsItem *>::Iterator iter = selected.begin();
        QPointF m(0,0); //магнитный момент выделенных частиц
        qreal minx,miny,maxx,maxy;
        while (iter!=selected.end()){
            temp = *iter;
            part = qgraphicsitem_cast<PartGraphicsItem *>(temp);
            if (iter==selected.begin()){
                minx = maxx = part->pos().x();
                miny = maxy = part->pos().y();
            } else {
                minx = qMin<qreal>(part->pos().x(),minx);
                miny = qMin<qreal>(part->pos().y(),miny);
                maxx = qMax<qreal>(part->pos().x(),maxx);
                maxy = qMax<qreal>(part->pos().y(),maxy);
            }
            m+=part->m();
            iter++;
        }
        circle->setM(m);
        circle->setPos((minx+maxx)/2.,(miny+maxy)/2.);
    }
}

void MyGraphicsScene::initSlider()
{
    if (_slider==NULL){
        this->_slider = new QWidget();
        QGridLayout *layout = new QGridLayout;
        layout->addWidget(new QLabel("fi"),0,0);
        layout->addWidget(new QLabel("m"),0,1);
        layout->addWidget(new QLabel("r"),0,2);
        _fiSlider = new QSlider();
        _fiSlider->setMinimum(0);
        _fiSlider->setMaximum(360*100);
        _fiSlider->connect(_fiSlider,SIGNAL(valueChanged(int)),this,SLOT(setFiValue(int)));

        _mSlider = new QSlider();
        _mSlider->setMinimum(10);
        _mSlider->setMaximum(30*100);
        _mSlider->connect(_mSlider,SIGNAL(valueChanged(int)),this,SLOT(setMValue(int)));


        _rSlider = new QSlider();
        _rSlider->setMinimum(0);
        _rSlider->setMaximum(99*100);

        layout->addWidget(_fiSlider,1,0);
        layout->addWidget(_mSlider,1,1);
        layout->addWidget(_rSlider,1,2);
        _slider->setLayout(layout);
        _slider->hide();
    }
    this->addWidget(_slider);
}

void MyGraphicsScene::showSlider()
{
    if (_slider==NULL) return;
    QList<QGraphicsItem *> selected = selectedItems();
    if (selected.size()>0){
        //ставим позицию слайдера
        PartGraphicsItem* part;
        QGraphicsItem* temp;
        QList<QGraphicsItem *>::Iterator iter = selected.begin();
        qreal minx,miny,maxx,maxy;
        QRectF partRect;
        while (iter!=selected.end()){
            temp = *iter;
            part = qgraphicsitem_cast<PartGraphicsItem *>(temp);
            partRect = part->boundingRect();
            if (iter==selected.begin()){
                minx = part->x() + partRect.left();
                maxx = part->x() + partRect.right();
                miny = part->y() + partRect.top();
                maxy = part->y() + partRect.bottom();
            } else {
                minx = qMin<qreal>(part->x() + partRect.left(),minx);
                miny = qMin<qreal>(part->y() + partRect.top(),miny);
                maxx = qMax<qreal>(part->x() + partRect.right(),maxx);
                maxy = qMax<qreal>(part->y() + partRect.bottom(),maxy);
            }
            iter++;
        }

        QRect pos = _slider->geometry();
        pos.moveLeft(maxx);
        pos.moveTop((miny+maxy)/2. - pos.height()/2.);
        _slider->setGeometry(pos);

        //ставим значение угла поворота и размера
        if (selected.size()==1){
            part = qgraphicsitem_cast<PartGraphicsItem *>(selected[0]);
            _rSlider->setValue(part->Radius()*100);
            _mSlider->setValue(part->M().length()*100);
            _fiSlider->setValue(part->M().grade()*100);
        }

        _slider->show();
    } else {
        _slider->hide();
    }
}

void MyGraphicsScene::hideSlider()
{
    _slider->hide();
}

void MyGraphicsScene::_addBorders(){
    this->addRect(this->sceneRect(),QPen(Qt::black));
}

void MyGraphicsScene::_addRule(){
    qreal w = this->width();
    qreal h = this->height();
    const int length = 3;
    for (qreal x = 0; x<=w; x+=getSize()){
        this->addLine(x,0,x,length,QPen(Qt::black));
        this->addLine(x,h,x,h-length,QPen(Qt::black));
    }

    for (qreal y = 0; y<=h; y+=getSize()){
        this->addLine(0,y,length,y,QPen(Qt::black));
        this->addLine(w,y,w-length,y,QPen(Qt::black));
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
