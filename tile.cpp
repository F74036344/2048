#include "tile.h"
#include "mainwindow.h"

#include <QGraphicsRectItem>
#include <QDebug>

extern MainWindow *w;
extern QGraphicsScene *gameAreaScene;

Tile::Tile(int row,int col,int current_value)
{
    qDebug() << "tile generated:" << " row: " << row <<" ,col: " << col ;
    //**Initialize the necessary values for tile
    boardEdgeSize = w->data->getBoardEdgeSizeValue();
    tileEdgeLength = w->gameview->getTileEdgeLengthValue();
    gap = (w->gameview->getGapValue());
    tileEdgeLengthTotal = (gap+tileEdgeLength)*boardEdgeSize + gap;
    //**Set the value for animation
    moveTimeTiny = 3;
    scaleTimeTiny = 15;
    emergeTimeTiny = 15;

    //moveDistanceTiny would be initialize whne move function is called
    scaleDistanceTiny = 1;
    emergeDistanceTiny = 1;

    moveTinyStepAmount = 85;
    scaleTinyStepAmount = 15;
    emergeTinyStepAmount = 40;

    timerForMove = new QTimer;
    timerForScale = new QTimer;
    timerForEmerge = new QTimer;
    //**Set the value for animation END

    //Initialize the current value
    setCurrentValue(current_value);
    //Also initialize the label which displays the currentValue
    label_demoValue = new QLabel;
    label_demoValue->setText(QString::number(getCurrentValue()));
    label_demoValue->setAlignment(Qt::AlignCenter);
    QFont font;
    font.setPointSize(32);
    font.setBold(true);
    label_demoValue->setFont(font);
    if(w->data->getTileColor() == QString("white"))
        label_demoValue->setStyleSheet("QLabel{background-color : transparent ;color : black}");
    else
        label_demoValue->setStyleSheet("QLabel{background-color : transparent ;color : white}");
    //Initialize current state
    setTileActing(false);

    //Initialize the visible items' information
    rectWidth = tileEdgeLength; rectHeight = tileEdgeLength;
    setRect(0,0,rectWidth,rectHeight);
    //Set tileRect's pos



    //tileRect's width and height would be set in collision() and emerge()


    label_demoValue->raise();

    if(current_value>2) //means that this *tile is generated by collision
    {
        collision();
    }
    else    //Otherwise, it's generated by created
    {
        qDebug() << "<tile.cpp>tileRect pos:" <<"x:"<<x()<<" ,y="<<y();
        qDebug() << "<tile.cpp>label pos:" <<"x:"<<label_demoValue->x()<<" ,y="<<label_demoValue->y();
        qDebug() << "<tile.cpp>tile generated:" << " row: " << row <<" ,col: " << col ;

        emerge();
    }
}

Tile::~Tile()
{
    delete timerForMove;
    delete timerForScale;
    delete timerForEmerge;

    delete label_demoValue;
}


void Tile::move(QString motion,int cell, bool isCollisionAnimationNeed)
{
    if(cell>0)
    {
        moveCellAmount = cell;
        moveDistanceTiny = 1 * moveCellAmount;
        if(!isCollisionAnimationNeed)
            connect(this,SIGNAL(moveIsDone()),this,SLOT(setTileActingFalse()));
        if(motion == QString("Up"))
        {
            counter = 0;
            connect(timerForMove,SIGNAL(timeout()),this,SLOT(moveUpTiny()));
            setTileActing(true);
            timerForMove->start(moveTimeTiny);
        }
        else if(motion == QString("Down"))
        {
            counter = 0;
            connect(timerForMove,SIGNAL(timeout()),this,SLOT(moveDownTiny()));
            setTileActing(true);
            timerForMove->start(moveTimeTiny);
        }
        else if(motion == QString("Left"))
        {
            counter = 0;
            connect(timerForMove,SIGNAL(timeout()),this,SLOT(moveLeftTiny()));
            setTileActing(true);
            timerForMove->start(moveTimeTiny);
        }
        else if(motion == QString("Right"))
        {
            counter = 0;
            connect(timerForMove,SIGNAL(timeout()),this,SLOT(moveRightTiny()));
            setTileActing(true);
            timerForMove->start(moveTimeTiny);
        }
    }
}
void Tile::moveUpTiny()
{
    counter++;
    setY(y()-moveDistanceTiny);
    label_demoValue->setGeometry(QRect(label_demoValue->x(),label_demoValue->y()-moveDistanceTiny,label_demoValue->width(),label_demoValue->height()));
    if(counter >= moveTinyStepAmount)
    {
        timerForMove->stop();
        emit moveIsDone();
    }
}
void Tile::moveDownTiny()
{
    counter++;
    setY(y()+moveDistanceTiny);
    label_demoValue->setGeometry(QRect(label_demoValue->x(),label_demoValue->y()+moveDistanceTiny,label_demoValue->width(),label_demoValue->height()));
    if(counter >= moveTinyStepAmount)
    {
        timerForMove->stop();
        emit moveIsDone();
    }
}
void Tile::moveLeftTiny()
{
    counter++;
    setX(x()-moveDistanceTiny);
    label_demoValue->setGeometry(QRect(label_demoValue->x()-moveDistanceTiny,label_demoValue->y(),label_demoValue->width(),label_demoValue->height()));
    if(counter >= moveTinyStepAmount)
    {
        timerForMove->stop();
        emit moveIsDone();
    }
}
void Tile::moveRightTiny()
{
    counter++;
    setX(x()+moveDistanceTiny);
    label_demoValue->setGeometry(QRect(label_demoValue->x()+moveDistanceTiny,label_demoValue->y(),label_demoValue->width(),label_demoValue->height()));
    if(counter >= moveTinyStepAmount)
    {
        timerForMove->stop();
        emit moveIsDone();
    }
}



void Tile::collision()
{
    //Initialize
    rectWidth = tileEdgeLength; rectHeight = tileEdgeLength;
    setRect(0,0,rectWidth,rectHeight);
    connect(this,SIGNAL(moveIsDone()),this,SLOT(setTileActingFalse()));

    //collision() contains scale larger and scale smaller
    //scale larger
    counter = 0;
    connect(timerForScale,SIGNAL(timeout()),this,SLOT(scaleEnlargeTiny()));
    timerForScale->start(scaleTimeTiny);
    counter = 0;
    //scale smaller
    connect(timerForScale,SIGNAL(timeout()),this,SLOT(scaleShrinkTiny()));
    timerForScale->start(scaleTimeTiny);

}
void Tile::scaleEnlargeTiny()
{
    counter++;
    rectWidth += 2*scaleDistanceTiny;   rectHeight+=2*scaleDistanceTiny;
    setRect(0,0,rectWidth,rectHeight);
    setPos(x()-scaleDistanceTiny,y()-scaleDistanceTiny);
    if(counter >= scaleTinyStepAmount)
    {
        timerForScale->stop();
        emit moveIsDone();
    }
}
void Tile::scaleShrinkTiny()
{
    counter++;
    rectWidth -= 2*scaleDistanceTiny;   rectHeight -= 2*scaleDistanceTiny;
    setRect(0,0,rectWidth,rectHeight);
    setPos(x()+scaleDistanceTiny,y()+scaleDistanceTiny);
    if(counter>=scaleTinyStepAmount)
    {
        timerForScale->stop();
        emit moveIsDone();
    }
}







void Tile::emerge()
{
    rectWidth = 0;  rectHeight = 0;
    setRect(0,0,rectWidth,rectHeight);
    counter = 0;
    connect(timerForEmerge,SIGNAL(timeout()),this,SLOT(emergeTiny()));
    setTileActing(true);
    timerForEmerge->start(emergeTimeTiny);
}
void Tile::emergeTiny()
{
    counter++;
    rectWidth += 2;  rectHeight += 2;
    setRect(0,0,rectWidth,rectHeight);
    setPos(x()-1,y()-1);
    if(counter>=emergeTinyStepAmount)
    {
        timerForEmerge->stop();
        setTileActing(false);
    }
}

void Tile::setTileActingFalse()
{
    setTileActing(false);
}




int Tile::getCurrentValue()
{
    return currentValue;
}
void Tile::setCurrentValue(int value)
{
    currentValue = value;
}

bool Tile::isTileActing()
{
    return tileActing;
}
void Tile::setTileActing(bool tof)
{
    tileActing = tof;
}



/*QRectF Tile::boundingRect()
{
    return QRectF(0,0,0,0);
}

void Tile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    setRect(0,0,tileEdgeLength,tileEdgeLength);
    QBrush brush(Qt::green);
    //painter->fillRect(this,brush);
}
*/
