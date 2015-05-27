#ifndef TILE_H
#define TILE_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QLabel>
#include <QPainter>
#include <QDebug>

class Tile :public QObject,public QGraphicsRectItem
{
    Q_OBJECT
public:
    Tile(int,int,int);
    ~Tile();


    void move(QString,int,bool);

    void collision();

    void emerge();

    bool isTileActing();
    void setTileActing(bool);

    int getCurrentValue();
    void setCurrentValue(int);

    QLabel *label_demoValue;

    /*QRectF boundingRect();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);*/

signals:
    void moveIsDone();



public slots:
    void moveUpTiny();
    void moveDownTiny();
    void moveLeftTiny();
    void moveRightTiny();

    void scaleEnlargeTiny();
    void scaleShrinkTiny();

    void emergeTiny();

    void setTileActingFalse();


private:
    bool tileActing;

    int currentValue;

    int boardEdgeSize;
    int tileEdgeLength;
    int gap;
    int tileEdgeLengthTotal;


    int rectWidth,rectHeight;

    int moveTimeTiny,scaleTimeTiny,emergeTimeTiny;//in ms
    int moveDistanceTiny,scaleDistanceTiny,emergeDistanceTiny;
    int moveTinyStepAmount,scaleTinyStepAmount,emergeTinyStepAmount;
    int moveCellAmount;
    QTimer *timerForMove, *timerForScale, *timerForEmerge;
    int counter;
};

#endif // TILE_H

