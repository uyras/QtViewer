#ifndef MYGRAPHICSSCENE_H
#define MYGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QKeyEvent>
#include <QtWidgets>
#include <vector>
#include "PartArray.h"
#include "partgraphicsitem.h"
#include "magneticcircle.h"

class PartGraphicsItem;
class MagneticCircle;

class MyGraphicsScene : public QGraphicsScene, public PartArray
{
    Q_OBJECT
public:
    explicit MyGraphicsScene(QObject *parent = 0);

    QPointF startDot, endDot;
    int E1;

    float getSize() const;
    void setSize(float value);
    void insert(Part *part);
    void attach(Part *part);

signals:
    void setM(double);
    void setE1(double);
    void setE2(double);

public slots:
    void addParts();
    void fullReDraw();
    //обновляет
    void reDraw();

    //действия с выделенной группой частиц
    void moveUp(float size);
    void moveDown(float size);
    void moveLeft(float size);
    void moveRight(float size);
    void setFiValue(int value);
    void setMValue(int value);
    void setRValue(int value);

    void clear();
    void resize(double x, double y);
    void dropRandom(double maxDestiny);
    void dropRandom(int count);
    void dropChain(double distance);
    void load(char *file, bool showNotifications=false);
    void addMagneticCircle(); //нарисовать магнитный круг вокруг выделенных частиц
    void removeMagneticCircle(); //удалить магнитный круг с рисунка
    void updateMagneticCirclePos(); //обновить позицию магнитного круга
    void initSlider();
    void showSlider();
    void hideSlider();


private:
    MagneticCircle* circle;
    float _size;
    void _addBorders();
    void _addRule();
    void keyPressEvent(QKeyEvent *event);
    QWidget* _slider;
    QSlider* _mSlider, *_fiSlider, *_rSlider;
};

#endif // MYGRAPHICSSCENE_H
