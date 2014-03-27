#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>

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

    
signals:
    
public slots:

    void scaleTo(int size);
    
};

#endif // MYGRAPHICSVIEW_H
