#ifndef TILECOMMANDER_H
#define TILECOMMANDER_H

#include <QGraphicsRectItem>
#include <QObject>
#include <QKeyEvent>
#include <QDebug>

#include "tile.h"

class TileCommander : public QObject,public QGraphicsRectItem
{
    Q_OBJECT
public:
    TileCommander();
    ~TileCommander();


   //遊戲大致流程:
   //1.先generateTile()
   //2.然後
   //step1:keyPressEvent():接收玩家的上下左右指令
   //step2:moveTile():根據所收到的上下左右指令移動tile
   //step3:generateTile():生成新的tile
   //step4:checkIfAnyTileReachGoal():檢查有沒有tile的value達到goal了，如果有則gameWin
   //step5:checkIfAnyTileIsMovable():檢查還有沒有可移動的tile，如果沒有則gameOver
   //然後再接續step1,周而復始，直至gameOver或gameWin
   void keyPressEvent(QKeyEvent *); //Receive key event from player
   void moveTile(QString); 	//Command tiles to move
    void generateTile();	//generate tile
    void checkIfAnyTileReachGoal(); //Check if any *tile's value reaches the goal. If is, then gameWin
    void checkIfAnyTileIsMovable();	//Check if all tiles are unmovable.If are, then gameOver

    void setTileAvailable(int ,bool);
    bool isTileAvailable(int);

    void setTileColor(QColor);
    QColor getTileColor();
    QLabel *gameStatusLabel;

    int power(int,int);


public slots:
    void changeToCollision();


private:
    int boardEdgeSize;
    int goal;
    int tileEdgeLength;
    int gap;
    int tileEdgeLengthTotal;
    bool *tileAvailable;

    int currentRow,currentCol;
    int nextRow,nextCol;

    Tile **tile;    //Use second order pointer to ensure tha the addresses are continuous
    //*(tile)(first oreder pointer) woulb be dynamically allocated
    Tile *tmpTile;

    QColor tileColor;

    /*QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);*/
};

#endif // TILECOMMANDER_H
