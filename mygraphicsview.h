#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QScrollBar>
#include <QDebug>
#include <QGraphicsOpacityEffect>

#include "mygraphicsscene.h"
#include "PartArray.h"
#include "partgraphicsitem.h"
#include "Part.h"
#include "random.h"

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MyGraphicsView(QObject *parent = 0);
    MyGraphicsScene* scene();
    MyGraphicsScene* _scene;

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
    void resizeEvent(QResizeEvent *event); //изменение положения превью при изменении размеров основного окна

private:
    QGraphicsView preview;
    
signals:
    void scaleUp();
    void scaleDown();
    
public slots:
    void updatePreview(const QList<QRectF> & regions);
    void scaleTo(int size);
    void changeOperateMode(int mode);
    void dbgSlot();
    void toggleDoubleArrows(bool on);
    
};

#endif // MYGRAPHICSVIEW_H
