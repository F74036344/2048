#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include "mainwindow.h"

#include <QLabel>
#include <QWidget>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QKeyEvent>

extern MainWindow* w;

namespace Ui {
class GameView;
}

class GameView : public QWidget
{
    Q_OBJECT

public:
    explicit GameView(QWidget *parent = 0);
    ~GameView();
    QGraphicsScene *gameAreaScene;

    //遊戲大致流程:
    //1.先generateTile()
    //2.然後
    //step1:keyPressEvent():接收玩家的上下左右指令
    //step2:moveTile():根據所收到的上下左右指令移動tile
    //step3:generateTile():生成新的tile
    //step4:tileAnimationImplementation():實作tile動畫(將step2,3之結果顯示出來)
    //step5:checkIfAnyTileReachGoal():檢查有沒有tile的value達到goal了，如果有則gameWin
    //step6:checkIfAnyTileIsMovable():檢查還有沒有可移動的tile，如果沒有則gameOver

    //然後再接續step1,周而復始，直至gameOver或gameWin

    void emitMoveTileSignalToAll(QString motion);
        void moveTile(int,QString);

    void generateTile();	//generate tile
    void tileAniMove();
    void tileAniEmerge();
    void checkIfAnyTileReachGoal(); //Check if any *tile's value reaches the goal. If is, then gameWin
    void checkIfAnyTileIsMovable();	//Check if all tiles are unmovable.If are, then gameOver


    void tileCreator(int index,int value);
        void tileTextCreator(int index,int value);
        void tileRectCreator(int index,int value);
    void tileDestructor(int value);

    void tmpDisplayEmergeTileInitializer(int selfIndex,int index,int value);
        void tmpDisplayEmergeTileRectInitializer(int selfIndex,int index,int value);
        //void tmpDisplayEmergeTileTextInitializer(int selfIndex,int index,int value);
    void tmpDisplayEmergeTileDestructor(int selfIndex);

    void scoreAddAndShow(int variation);

    void gameStatusLabelCreator();
    void gameStatusLabelDestructor();

    void setTileColor(QString color);
    QColor getTileColor(int tileValue);

    void setStopWatchValueAndShow(int totalSecond);

    int getTileEdgeLengthValue();
    int getGapValue();

    int getCurrentValueOfTile(int);
    void setCurrentValueOfTile(int,int);

    int power(int a,int n);


    void gameOver();
    void gameWin();

protected:
    void keyPressEvent(QKeyEvent *);    //Receive key event from player


private slots:

    void on_pushButton_IDontWantToPlay_clicked();

    void on_pushButton_restartTheGame_clicked();

    void on_pushButton_goBackToMenu_clicked();

    void on_pushButton_quiet_clicked();

    void on_pushButton_pause_clicked();

    void oneTimeUnitPass();

    void tileAniMoveImplementation();

    void tileAniEmergeImplementation();

private:

    QGraphicsRectItem **tile;
    QLabel **label_demoValue;
    //game view properties
    Ui::GameView *ui;
    QGraphicsRectItem *rectBlock;
    QGraphicsRectItem gameAreaFrame;
    QLabel *gameStatusLabel;
    int controlPanelWidth,controlPanelHeight;
    int gameAreaPosX,gameAreaPosY;
    const int boardEdgeSize = w->data->getBoardEdgeSizeValue();
    int goal;
    int timeLeft;   //in second
    bool keyEventBlock;
    bool isAnyTileMoved;
    bool isBackgroundMusicOn,isGamePaused;

    //tile properties
    int tileEdgeLength,gameAreaEdgeLength,gap;
    QString tileColor;
    int *currentValueOfTile,*nextValueOfTile,*nextPosOfTile;   //為了做tile動畫，所以需要兩個array分別存移動前，移動後tile之位置與值
    int score;

    int totalTileAmount;

    //tile animation properties
    bool *isThisIndexGenerateNewTile;
    int newTileAmount;
    QGraphicsRectItem *tmpDisplayEmergeTile;
    int tmpDisplayEmergeTileEdgeLength;
    QLabel *tmpDisplayEmergeLabel;
    int tmpDisplayEmergeLabelFontSize;
    QTimer *timerForMove;
    QTimer *timerForEmerge;
    QTimer *timerForStopWatch;
    int counterForMoveStep,counterForEmergeStep,counterForStopWatch;
};

#endif // GAMEVIEW_H
