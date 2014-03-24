#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QMouseEvent>

#include "PartArray.h"
#include "partgraphicsitem.h"
#include "Part.h"

class MyGraphicsView : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MyGraphicsView(QObject *parent = 0);

    QPoint startDot, endDot;
    QGraphicsLineItem *pLine = NULL;

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    
signals:
    void setM(double);
    void setE1(double);
    void setE2(double);
    
public slots:
    //очищает указатели мыши
    void clearMousePointers();
	//обновляет
    void reDraw(PartArray*);
    void fullReDraw(PartArray*);
    void scaleTo(int size);
    
};

#endif // MYGRAPHICSVIEW_H
