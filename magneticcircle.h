#ifndef MAGNETICCIRCLE_H
#define MAGNETICCIRCLE_H

#include <QGraphicsItem>
#include <QObject>
#include <QGraphicsScene>
#include <QList>
#include <mygraphicsscene.h>

class MagneticCircle : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    explicit MagneticCircle(float size = 20, QObject *parent = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QPointF M() const;
    void setM(const QPointF &value);

signals:

public slots:

private:
    float size;
    QPointF m;

};

#endif // MAGNETICCIRCLE_H
