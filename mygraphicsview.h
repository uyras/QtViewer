#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QScrollBar>

#include "mygraphicsscene.h"
#include "PartArray.h"
#include "partgraphicsitem.h"
#include "Part.h"

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

    
signals:
    void scaleUp();
    void scaleDown();
    
public slots:

    void scaleTo(int size);
    
};

#endif // MYGRAPHICSVIEW_H
