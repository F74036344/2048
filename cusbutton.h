#ifndef CUSBUTTON_H
#define CUSBUTTON_H

#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>

class CusButton : public QGraphicsItem
{
public:
    CusButton();
    ~CusButton();

    QRectF boundingRect() const;
    void paint(QPainter *painter,
               QStyleOptionGraphicsItem *option,
               QWidget *widget);
    bool Pressed;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

};

#endif // CUSBUTTON_H
