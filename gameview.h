#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QLabel>
#include <QWidget>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QKeyEvent>


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
    //step4:checkIfAnyTileReachGoal():檢查有沒有tile的value達到goal了，如果有則gameWin
    //step5:checkIfAnyTileIsMovable():檢查還有沒有可移動的tile，如果沒有則gameOver
    //然後再接續step1,周而復始，直至gameOver或gameWin
    void emitMoveTileSignalToAll(QString);
        void moveTile(int,QString);

    void generateTile();	//generate tile
    void checkIfAnyTileReachGoal(); //Check if any *tile's value reaches the goal. If is, then gameWin
    void checkIfAnyTileIsMovable();	//Check if all tiles are unmovable.If are, then gameOver


    void tileCreator(int);
        void tileTextCreator(int,int);
        void tileRectCreator(int);
    void tileDestructor(int);

    void scoreAddAndShow(int);

    void setTileColor(QColor);
    QColor getTileColor();

    void setStopWatchValueAndShow(int);

    int getTileEdgeLengthValue();
    int getGapValue();

    int getCurrentValueOfTile(int);
    void setCurrentValueOfTile(int,int);

    int power(int,int);


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

private:

    QGraphicsRectItem **tile;
    QLabel **label_demoValue;
    //game view properties
    Ui::GameView *ui;
    QGraphicsRectItem *rectBlock;
    QGraphicsRectItem gameAreaFrame;
    QLabel gameStatusLabel;
    int controlPanelWidth,controlPanelHeight;
    int gameAreaPosX,gameAreaPosY;
    int boardEdgeSize;
    int goal;
    int timeLeft;   //in second
    bool keyEventBlock;
    bool isAnyTileMoved;
    bool isBackgroundMusicOn,isGamePaused;

    //tile properties
    int tileEdgeLength,gameAreaEdgeLength,gap;
    QColor tileColor;
    int *currentValueOfTile;
    int score;

    //tile animation properties
    bool *tileIsActing;
    int moveTimeTiny,scaleTimeTiny,emergeTimeTiny;//in ms
    int moveDistanceTiny,scaleDistanceTiny,emergeDistanceTiny;
    int moveTinyStepAmount,scaleTinyStepAmount,emergeTinyStepAmount;

    QTimer *timerForMove, *timerForScale, *timerForEmerge;
    QTimer *timerForStopWatch;
    int counterForStopWatch;
};

#endif // GAMEVIEW_H
