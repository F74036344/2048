#include "cusbutton.h"

CusButton::CusButton()
{
    Pressed = false;
    setFlag(ItemIsMovable);
}

CusButton::~CusButton()
{

}

QRectF CusButton::boundingRect() const
{
    return QRectF(0,0,100,100);
}

void CusButton::paint(QPainter *painter, QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec =boundingRect();
    QBrush brush(Qt::blue);

    if(Pressed)
    {
        brush.setColor(Qt::red);
    }
    else
    {
        brush.setColor(Qt::green);
    }

    painter->fillRect(rec,brush);
    painter->drawRect(rec);

}

void CusButton::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Pressed = true;
    update();
    QGraphicsItem::mousePressEvent(event);
}

void CusButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

}

