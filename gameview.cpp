#include "gameview.h"
#include "ui_gameview.h"
#include "mainwindow.h"

#include <QtGui>
#include <QtCore>
#include <QMessageBox>



extern MainWindow *w;


GameView::GameView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameView)
{
    ui->setupUi(this);

    timeLeft = w->data->getTimeLimitValue();
    if(w->data->isTimeLimitChecked() && timeLeft<=20)
    {
            w->sound->soundBgMusicPlay(w->sound->gameviewBackgroundMusicHurry,34500);
    }
    else
        w->sound->soundBgMusicPlay(w->sound->gameviewBackgroundMusic,76800);

    isBackgroundMusicOn = true;
    isGamePaused = false;

    keyEventBlock = false;

    //Initialize the control panel properties
    controlPanelWidth = 120;
    controlPanelHeight = 400;

    //Initialize the basic properties
    boardEdgeSize = w->data->getBoardEdgeSizeValue();
    gap = 5;
    tileEdgeLength = 80;
    gameAreaEdgeLength = (gap+tileEdgeLength)*(w->data->getBoardEdgeSizeValue())+gap;
    goal = w->data->getGoalValue();
    setFixedSize(gap + controlPanelWidth + ((gap+tileEdgeLength) * (w->data->getBoardEdgeSizeValue())+gap),
                       2 * gap + ((gap+tileEdgeLength)*(w->data->getBoardEdgeSizeValue())+gap));
    ui->label_scoreText->setGeometry(0,0,controlPanelWidth,30);
    ui->label_scoreValue->setGeometry(0,30,controlPanelWidth,30);
    ui->label_goalText->setGeometry(0,70,controlPanelWidth,30);
    ui->label_goalValue->setGeometry(0,100,controlPanelWidth,30);
    ui->label_timeText->setGeometry(0,140,controlPanelWidth,30);
    ui->label_timeValue->setGeometry(0,170,controlPanelWidth,30);


    ui->pushButton_restartTheGame->setGeometry(-30,
                                             -30,
                                             0,
                                             0);
    ui->pushButton_IDontWantToPlay->setGeometry(8,
                                             gap+(w->data->getBoardEdgeSizeValue()-2)*(tileEdgeLength+gap)+tileEdgeLength/2+45,
                                             controlPanelWidth-8-8,
                                             50);
    ui->pushButton_goBackToMenu->setGeometry(8,
                                             gap+(w->data->getBoardEdgeSizeValue()-1)*(tileEdgeLength+gap)+tileEdgeLength/2+15,
                                             controlPanelWidth-8-8,
                                             30);
    ui->pushButton_quiet->setGeometry(10,
                                      gap+(w->data->getBoardEdgeSizeValue()-2)*(tileEdgeLength+gap)+tileEdgeLength/2,
                                      40,
                                      40);
    ui->pushButton_pause->setGeometry(70,
                                      gap+(w->data->getBoardEdgeSizeValue()-2)*(tileEdgeLength+gap)+tileEdgeLength/2,
                                      40,
                                      40);
    setTileColor(w->data->getTileColor());

    score = 0;
    ui->label_scoreValue->setText(QString::number(score));
    ui->label_scoreValue->setStyleSheet("QLabel{background-color : transparent ; color : blue}");
    ui->label_scoreValue->setAlignment(Qt::AlignCenter);


    ui->label_goalValue->setText(QString::number(w->data->getGoalValue()));
    ui->label_goalValue->setStyleSheet("QLabel{background-color : transparent ; color : red}");
    ui->label_goalValue->setAlignment(Qt::AlignCenter);

    //Set stopwatch properties
    timerForStopWatch = new QTimer;
    if(w->data->isTimeLimitChecked())
    {
        setStopWatchValueAndShow(timeLeft);

        counterForStopWatch = 0;
        connect(timerForStopWatch,SIGNAL(timeout()),this,SLOT(oneTimeUnitPass()));
        timerForStopWatch->start(50);
    }
    else
        ui->label_timeValue->setText("∞");
    ui->label_timeValue->setStyleSheet("QLabel{background-color : transparent ; color : green}");
    ui->label_timeValue->setAlignment(Qt::AlignCenter);


    //**Set the properties for animation START
    tileIsActing = new bool[power(boardEdgeSize,2)];
    for(int i=0;i<power(boardEdgeSize,2);i++)
        *tileIsActing = false;
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

    //**Set the properties for animation END



    //gameArea origin : (controlPanelWidth,gap)
    gameAreaPosX = controlPanelWidth;
    gameAreaPosY = gap;
    rectBlock = new QGraphicsRectItem[power(w->data->getBoardEdgeSizeValue(),2)];

    //create object: tile, label_demoValue

    tile = new QGraphicsRectItem*[power(boardEdgeSize,2)];
    for(int i=0;i<power(boardEdgeSize,2);i++)
    {
        (*(tile+i)) = NULL;   //NULL means that this *tile doesn't hold **tile
    }

    label_demoValue = new QLabel*[power(boardEdgeSize,2)];
    for(int i=0;i<power(boardEdgeSize,2);i++)
        (*(label_demoValue+i)) = NULL;
    currentValueOfTile = new int[power(boardEdgeSize,2)];


    //create a scene and set its properties
    gameAreaScene = new QGraphicsScene;
    gameAreaScene->setSceneRect(0,0,(gap+tileEdgeLength)*(w->data->getBoardEdgeSizeValue())+gap-2,
                        (gap+tileEdgeLength)*(w->data->getBoardEdgeSizeValue())+gap-2);
    //set graphicsView properties and set scene to view
    ui->graphicsView->setGeometry(gameAreaPosX,gameAreaPosY,gameAreaEdgeLength,gameAreaEdgeLength);
    ui->graphicsView->setScene(gameAreaScene);

    //Initialize gameStatusLabel, gameStatusBackground
    gameStatusLabel.setGeometry(0,0,gameAreaEdgeLength,gameAreaEdgeLength);
    gameStatusLabel.setAlignment(Qt::AlignCenter);
    gameStatusLabel.setStyleSheet("QLabel{background-color : transparent ; color : transparent}");

    //The object above would be added to scene later

    //Start to set rectBlock
    for(int row = 0;row<(w->data->getBoardEdgeSizeValue());row++)
        for(int col = 0;col<(w->data->getBoardEdgeSizeValue());col++)
        {
            //set each rectBlock's rect
            rectBlock[row+col*(w->data->getBoardEdgeSizeValue())] . setRect(0,0,tileEdgeLength,tileEdgeLength);
            rectBlock[row+col*(w->data->getBoardEdgeSizeValue())] . setPos(gap + col * (tileEdgeLength+gap),gap + row * (tileEdgeLength+gap));
            //add rectBlocks to the scene
            gameAreaScene->addItem(rectBlock+row+col*(w->data->getBoardEdgeSizeValue()));
            qDebug() << "Add rectBlock "<<row+col*(w->data->getBoardEdgeSizeValue());

          }
    //Basic game scene had been set up above,
    //Now it's about time to set the gameAreaFrame
    gameAreaFrame.setRect(0,0,
            (gap+tileEdgeLength)*(w->data->getBoardEdgeSizeValue())+gap-3,
            (gap+tileEdgeLength)*(w->data->getBoardEdgeSizeValue())+gap-3);
    gameAreaFrame.setPen(QPen(getTileColor(),3));
    gameAreaFrame.setBrush(Qt::transparent);
    //setFlag(QGraphicsItem::ItemIsFocusable);
    //setFocus();
    gameAreaScene->addItem(&gameAreaFrame);
    gameAreaFrame.setPos(0,0);




    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //qDebug()<<"<gameview.cpp>graphicsView show";
    setFocusPolicy(Qt::ClickFocus);
    generateTile();
}

GameView::~GameView()
{
    w->sound->soundBgMusicPlay_stop();
    delete ui;
    delete [] rectBlock;
    delete timerForEmerge;
    delete timerForMove;
    delete timerForScale;
    delete [] tileIsActing;
    delete timerForStopWatch;
    //delete gameAreaScene;

}

void GameView::keyPressEvent(QKeyEvent *event)  //step 1:receive key event from player
{
    //會傳event的情況:  1.尚未gameEnd(gameOver or gameWin)
                    qDebug() << event->key()<<"Key detected";
    if(!keyEventBlock)    //game尚未結束，則傳event
    {

            if(event->key()==Qt::Key_Up||event->key()==Qt::Key_W)
            {
                emitMoveTileSignalToAll(QString("Up"));
            }
            else if(event->key()==Qt::Key_Down||event->key()==Qt::Key_S)
            {
                emitMoveTileSignalToAll(QString("Down"));
            }
            else if(event->key()==Qt::Key_Left||event->key()==Qt::Key_A)
            {
                emitMoveTileSignalToAll(QString("Left"));
            }
            else if(event->key()==Qt::Key_Right||event->key()==Qt::Key_D)
            {
                emitMoveTileSignalToAll(QString("Right"));
            }


    }
}
void GameView::emitMoveTileSignalToAll(QString motion)
{
    isAnyTileMoved = false;
    if(motion == QString("Up"))
    {
        qDebug() << "motion Up detected";
        //由上往下發指令
        for(int row=0;row<boardEdgeSize;++row)
            for(int col=0;col<boardEdgeSize;++col)
                moveTile(col+row*boardEdgeSize,motion);  //Would trigger function moveTile()
    }
    else if(motion == QString("Down"))
    {
        //由下往上發指令
        for(int row=boardEdgeSize-1;row>=0;--row)
            for(int col=0;col<boardEdgeSize;++col)
                moveTile(col+row*boardEdgeSize,motion);  //Would trigger function moveTile()
    }
    else if(motion == QString("Left"))
    {
        //由左往右發指令
        for(int col=0;col<boardEdgeSize;++col)
            for(int row=0;row<boardEdgeSize;++row)
                moveTile(col+row*boardEdgeSize,motion);  //Would trigger function moveTile()}
    }
    else if(motion == QString("Right"))
    {
        //由右往左發指令
        for(int col=boardEdgeSize-1;col>=0;--col)
            for(int row=0;row<boardEdgeSize;++row)
                moveTile(col+row*boardEdgeSize,motion);  //Would trigger function moveTile()
    }

    if(isAnyTileMoved)  //If any tile is moved, then generate tile;Otherwise do nothing
    {
        generateTile();
    }

}



void GameView::moveTile(int index,QString motion) //step 2:move tiles
//this function moves a tile only, so moving all the tiles need to call this function several times
{
    if((*(tile+index)) == NULL)
        ;   //Do nothing
    else
    {
        int row,col,currentRow,currentCol;
        int step = 0;
        if(motion == QString("Up"))
        {
                    qDebug() << "motion Up received";
            row = index/boardEdgeSize;
            col = index%boardEdgeSize;
            currentRow = row;
            currentCol = col;
            //由該tile往上檢查
            while(1)
            {
                row--;
                if(row<0)
                    break;
                if((*(tile+col+row*boardEdgeSize)) == NULL)   //Available
                    step++;
                else
                {
                    if((*(currentValueOfTile+currentCol+currentRow*boardEdgeSize)) == (*(currentValueOfTile+col+row*boardEdgeSize)))
                    {
                        step++;
                        break;
                    }
                    else
                        break;
                }
            }
            if(step>0)
            {
                isAnyTileMoved = true;
                if((*(tile+currentCol+(currentRow-step)*boardEdgeSize)) == NULL)
                {
                    (*(currentValueOfTile+currentCol+(currentRow-step)*boardEdgeSize)) = (*(currentValueOfTile+currentCol+currentRow*boardEdgeSize));
                    tileCreator(currentCol+(currentRow-step)*boardEdgeSize);
                    tileDestructor(currentCol+currentRow*boardEdgeSize);
                }
                else    // target!=NULL
                {
                    (*(currentValueOfTile+currentCol+(currentRow-step)*boardEdgeSize)) += (*(currentValueOfTile+currentCol+currentRow*boardEdgeSize));
                    tileDestructor(currentCol+currentRow*boardEdgeSize);
                    tileCreator(currentCol+(currentRow-step)*boardEdgeSize);
                    scoreAddAndShow(*(currentValueOfTile+currentCol+(currentRow-step)*boardEdgeSize));
                }
            }

        }
        else if(motion == QString("Down"))
        {
            row = index/boardEdgeSize;
            col = index%boardEdgeSize;
            currentRow = row;
            currentCol = col;
            //由該tile往下檢查
            while(1)
            {
                row++;
                if(row>=boardEdgeSize)
                    break;
                if((*(tile+col+row*boardEdgeSize)) == NULL)   //Available
                    step++;
                else
                {
                    if((*(currentValueOfTile+currentCol+currentRow*boardEdgeSize)) == (*(currentValueOfTile+col+row*boardEdgeSize)))
                    {
                        step++;
                        break;
                    }
                    else
                        break;
                }
            }
            if(step>0)
            {
                isAnyTileMoved = true;
                if((*(tile+currentCol+(currentRow+step)*boardEdgeSize)) == NULL)
                {
                    (*(currentValueOfTile+currentCol+(currentRow+step)*boardEdgeSize)) = (*(currentValueOfTile+currentCol+currentRow*boardEdgeSize));
                    tileCreator(currentCol+(currentRow+step)*boardEdgeSize);
                    tileDestructor(currentCol+currentRow*boardEdgeSize);
                }
                else    // target!=NULL
                {
                    (*(currentValueOfTile+currentCol+(currentRow+step)*boardEdgeSize)) += (*(currentValueOfTile+currentCol+currentRow*boardEdgeSize));
                    tileCreator(currentCol+(currentRow+step)*boardEdgeSize);
                    tileDestructor(currentCol+currentRow*boardEdgeSize);
                    scoreAddAndShow(*(currentValueOfTile+currentCol+(currentRow+step)*boardEdgeSize));
                }
            }

        }
        else if(motion == QString("Left"))
        {
            row = index/boardEdgeSize;
            col = index%boardEdgeSize;
            currentRow = row;
            currentCol = col;
            //由該tile往左檢查

            while(1)
            {
                col--;
                if(col<0)
                    break;
                if((*(tile+col+row*boardEdgeSize)) == NULL)   //Available
                    step++;
                else
                {
                    if((*(currentValueOfTile+currentCol+currentRow*boardEdgeSize)) == (*(currentValueOfTile+col+row*boardEdgeSize)))
                    {
                        step++;
                        break;
                    }
                    else
                        break;
                }
            }
            if(step>0)
            {
                isAnyTileMoved = true;
                if((*(tile+(currentCol-step)+currentRow*boardEdgeSize)) == NULL)
                {
                    (*(currentValueOfTile+(currentCol-step)+currentRow*boardEdgeSize)) = (*(currentValueOfTile+currentCol+currentRow*boardEdgeSize));
                    tileCreator((currentCol-step)+currentRow*boardEdgeSize);
                    tileDestructor(currentCol+currentRow*boardEdgeSize);
                }
                else    // target!=NULL
                {
                    (*(currentValueOfTile+(currentCol-step)+currentRow*boardEdgeSize)) += (*(currentValueOfTile+currentCol+currentRow*boardEdgeSize));
                    tileCreator((currentCol-step)+currentRow*boardEdgeSize);
                    tileDestructor(currentCol+currentRow*boardEdgeSize);
                    scoreAddAndShow(*(currentValueOfTile+(currentCol-step)+currentRow*boardEdgeSize));
                }
            }

        }
        else if(motion == QString("Right"))
        {
            row = index/boardEdgeSize;
            col = index%boardEdgeSize;
            currentRow = row;
            currentCol = col;
            //由該tile往右檢查
            while(1)
            {
                col++;
                if(col>=boardEdgeSize)
                    break;
                if((*(tile+col+row*boardEdgeSize)) == NULL)   //Available
                    step++;
                else
                {
                    if((*(currentValueOfTile+currentCol+currentRow*boardEdgeSize)) == (*(currentValueOfTile+col+row*boardEdgeSize)))
                    {
                        step++;
                        break;
                    }
                    else
                        break;
                }
            }
            if(step>0)
            {
                isAnyTileMoved = true;
                if((*(tile+(currentCol+step)+currentRow*boardEdgeSize) == NULL))
                {
                    (*(currentValueOfTile+(currentCol+step)+currentRow*boardEdgeSize)) = (*(currentValueOfTile+currentCol+currentRow*boardEdgeSize));
                    tileCreator((currentCol+step)+currentRow*boardEdgeSize);
                    tileDestructor(currentCol+currentRow*boardEdgeSize);
                }
                else    // target!=NULL
                {
                    (*(currentValueOfTile+(currentCol+step)+currentRow*boardEdgeSize)) += (*(currentValueOfTile+currentCol+currentRow*boardEdgeSize));
                    tileCreator((currentCol+step)+currentRow*boardEdgeSize);
                    tileDestructor(currentCol+currentRow*boardEdgeSize);
                    scoreAddAndShow(*(currentValueOfTile+(currentCol+step)+currentRow*boardEdgeSize));
                }
            }

        }
    }
}

void GameView::generateTile()   //step 3
{

    int ballot[power(boardEdgeSize,2)];
    int current_index=0;
    //製籤
    for(int i=0;i<power(boardEdgeSize,2);i++)
    {
        if(*(tile+i)==NULL) //Available
        {
            //將空的*tile加到ballot中
            ballot[current_index]=i;
            current_index++;

        }
    }
    //total_index_available_amount:可使用的籤的個數，即可配置的位置的個數
    int total_index_available_amount = current_index;
    //開始產生*tile(開始抽籤以決定要產生的位置
    //先設定要產生幾個*tile
    //從data讀取每一步要產生的*tile的個數
    int tileAmountToGenerate = (w->data->getTileGenerateAmount());
    for(int i=0;i<tileAmountToGenerate;i++)
    {
        if(total_index_available_amount <= 0)
        {
    //當可產生的位置數量=0時，跳出迴圈，不再生成新的*tile
            break;
        }
        current_index = qrand()%(total_index_available_amount);
        //Assign a new **tile to *tile available
        //並傳給tileCreator其位置資訊(index)

        (*(currentValueOfTile+ballot[current_index])) = 2;  //先設定初值
        tileCreator(ballot[current_index]); //依照index產生新的tile和label_demoValue

        //籤已被用走一個->該籤需從ballot中刪掉
        for(int j=current_index;j<total_index_available_amount-1;j++)
            ballot[j] = ballot[j+1];

        //已配置一個新的*tile，so total_index_available_amount -1;
        total_index_available_amount--;
    }
    qDebug() << "Step3:generateTile done";
    checkIfAnyTileReachGoal();  //進入step 4
}

void GameView::checkIfAnyTileReachGoal()    //step 4
{
    for(int i=0;i<power(boardEdgeSize,2);i++)
    {

        if(*(tile+i) !=  NULL)     //檢查沒有空位(即有tile)的*tile 的tileCurrentValue是否已到達目標
        {
            if( getCurrentValueOfTile(i) >= goal)
            {
                gameWin();
                break;

            }
        }
    }
        qDebug() << "Step4:checkIfAnyTileReachGoal done";
    checkIfAnyTileIsMovable();	//進入step5:檢查還有沒有可以動的*tile
}

void GameView::checkIfAnyTileIsMovable()
{
    //檢查是否所有*tile皆已不能動
       bool allTileCanNotMove = true;
       //向右及向下檢查是否有1.空的tile 2.同樣value的tile存在
       //有的話就代表tile還可以移動，allTileCanNotMove設為false
       for(int row=0;row<boardEdgeSize;row++)
           for(int col=0;col<boardEdgeSize;col++)
           {
               //向右檢查
               if(col+1>=boardEdgeSize)	//如果右邊沒東西就不用檢查
                   ;//do nothing
               else
               {
                   //如果有空的tile，就把allTileCanNotMove設為false
                   if(*(tile+(col+1)+row*boardEdgeSize)==NULL)  //Available
                       allTileCanNotMove =  false;

                   //如果有相同value的，就把allTileCanNotMove設為false
                   if((*(currentValueOfTile+col+row*boardEdgeSize))==(*(currentValueOfTile+(col+1)+row*boardEdgeSize)))
                       allTileCanNotMove = false;
               }
               //向下檢查
               if(row+1>=boardEdgeSize)	//如果下面沒東西就不用檢查
                   ;//do nothing
               else
               {
                   //如果有空的tile，就把allTileCanNotMove設為false
                   if(*(tile+col+(row+1)*boardEdgeSize)==NULL)  //Available
                       allTileCanNotMove = false;
                   //如果有相同value的，就把allTileCanNotMove設為false
                   if((*(currentValueOfTile+col+row*boardEdgeSize))==(*(currentValueOfTile+col+(row+1)*boardEdgeSize)))
                       allTileCanNotMove = false;
               }

           }

       if(allTileCanNotMove)
       {
           //如果所有*tile皆不能動，則gameOver
           gameOver();
       }
       //如果還有*tile能動的話，則沒有動作退出此函式，等待下一次的keyPressEvent

            qDebug() << "Step5:checkIfAnyTileMovable done";
}

void GameView::tileCreator(int index)
{
    //1.刷新label_demoValue's value、position
    //2.刷新tile's rect's position
    tileRectCreator(index);
    tileTextCreator(index,*(currentValueOfTile+index));
}

void GameView::tileTextCreator(int index,int value)
{
    int row = index/boardEdgeSize;
    int col = index%boardEdgeSize;
    (*(label_demoValue+index)) = new QLabel;

    (*(label_demoValue+index))->setGeometry(gap+(gap+tileEdgeLength)*col,gap+(gap+tileEdgeLength)*row,tileEdgeLength,tileEdgeLength);
    (*(label_demoValue+index))->setText(QString::number(value));
    QFont font;
    if(value >= 1000)
        font.setPointSize(24);
    else if(value >= 100)
        font.setPointSize(28);
    else
        font.setPointSize(32);
    font.setBold(true);
    (*(label_demoValue+index))->setFont(font);
    if(getTileColor()==QColor(Qt::white))
        (*(label_demoValue+index))->setStyleSheet("QLabel{background-color : transparent ; color : black}");
    else
        (*(label_demoValue+index))->setStyleSheet("QLabel{background-color : transparent ; color : white}");
    (*(label_demoValue+index))->setAlignment(Qt::AlignCenter);
    gameAreaScene->addWidget((*(label_demoValue+index)));
}

void GameView::tileRectCreator(int index)
{
    int row = index/boardEdgeSize;
    int col = index%boardEdgeSize;
    (*(tile+index)) = new QGraphicsRectItem;
    (*(tile+index))->setRect(0,0,tileEdgeLength,tileEdgeLength);
    (*(tile+index))->setPos(gap+(gap+tileEdgeLength)*col,gap+(gap+tileEdgeLength)*row);
    //依照tileColor來決定pen(外框)的顏色
    if(getTileColor() == QColor(Qt::white))
        (*(tile+index))->setPen(QPen(Qt::black,3));
    else
        (*(tile+index))->setPen(QPen(getTileColor(),1));
    (*(tile+index))->setBrush(getTileColor());
    gameAreaScene->addItem((*(tile+index)));
}

void GameView::tileDestructor(int index)
{
    delete (*(tile+index));
    *(tile+index) = NULL;
    delete (*(label_demoValue+index));
    *(label_demoValue+index) = NULL;
}

void GameView::scoreAddAndShow(int variation)
{
    score += variation;
    ui->label_scoreValue->setText(QString::number(score));
}

void GameView::setTileColor(QColor color)
{
    tileColor = color;
}

QColor GameView::getTileColor()
{
    return tileColor;
}

void GameView::oneTimeUnitPass()
{
    counterForStopWatch++;
    if(counterForStopWatch >= 20)
    {
        timeLeft--;
        setStopWatchValueAndShow(timeLeft);
        counterForStopWatch = 0;

    }
    if(timeLeft == 20)
    {
        if(counterForStopWatch==0)
        {
            w->sound->soundBgMusicPlay_stop();
            w->sound->soundBgMusicPlay(w->sound->gameviewBackgroundMusicHurry,34500);
        }
    }


    //The following are for text twinkling effect
    if(timeLeft <= 20)
    {
        if(timeLeft > 10)
        {
                ui->label_timeValue->setStyleSheet("QLabel{background-color : transparent ; color : orange}");

        }
        else if(timeLeft > 5)
        {
            if(counterForStopWatch%10 == 0)
                ui->label_timeValue->setStyleSheet("QLabel{background-color : transparent ; color : yellow}");
            else if(counterForStopWatch%10 == 5)
                ui->label_timeValue->setStyleSheet("QLabel{background-color : transparent ; color : orange}");


        }
        else if(timeLeft > 0)
        {
            if(counterForStopWatch%4 == 0)
                ui->label_timeValue->setStyleSheet("QLabel{background-color : transparent ; color : red}");
            else if(counterForStopWatch%4 == 2)
                ui->label_timeValue->setStyleSheet("QLabel{background-color : transparent ; color : yellow}");
        }
        else
        {
            ui->label_timeValue->setStyleSheet("QLabel{background-color : transparent ; color : red}");
            gameOver();
        }
    }
}

void GameView::setStopWatchValueAndShow(int totalSecond)
{
    int min,sec;
    min = totalSecond/60;
    sec = totalSecond%60;
    if(sec < 10)
        ui->label_timeValue->setText(QString::number(min)+":0"+QString::number(sec));
    else
        ui->label_timeValue->setText(QString::number(min)+":"+QString::number(sec));
}



int GameView::getTileEdgeLengthValue()
{
    return tileEdgeLength;
}


int GameView::getGapValue()
{
    return gap;
}

int GameView::getCurrentValueOfTile(int index)
{
    return *(currentValueOfTile+index);
}

void GameView::setCurrentValueOfTile(int index, int value)
{
    *(currentValueOfTile+index) = value;
}


int GameView::power(int a, int n)
{
    int sum = 1;
    for(int i=0;i<n;i++)
        sum = sum * a;
    return sum;
}

void GameView::gameOver()
{
    w->sound->soundPlay_stop();
    w->sound->soundBgMusicPlay_stop();
    w->sound->soundPlay(w->sound->gameOverMusic);
    timerForStopWatch->stop();
    keyEventBlock = true;
    gameStatusLabel.setText("Game Over\n(oДo)\"");
    QFont font;
    font.setPointSize(28);
    font.setBold(true);
    gameStatusLabel.setFont(font);
    gameStatusLabel.setStyleSheet("QLabel{background-color : QColor(0,0,0,70) ; color : red}");

    gameAreaScene->addWidget(&gameStatusLabel);
    ui->pushButton_restartTheGame->setGeometry(controlPanelWidth+gap+(gap+tileEdgeLength)*(boardEdgeSize/2.0-0.75),
                                               gap+(gap+tileEdgeLength)*(boardEdgeSize/2.0+0.8),
                                               gap+tileEdgeLength*1.5,
                                               40);
    ui->pushButton_pause->setEnabled(false);
    ui->pushButton_quiet->setEnabled(false);
}

void GameView::gameWin()
{
    w->sound->soundBgMusicPlay_stop();
    w->sound->soundPlay(w->sound->gameWinMusic);
    timerForStopWatch->stop();
    keyEventBlock = true;
    gameStatusLabel.setText("You Win The Game\n\\^o^/");
    QFont font;
    font.setPointSize(28);
    font.setBold(true);
    gameStatusLabel.setFont(font);
    gameStatusLabel.setStyleSheet("QLabel{background-color : QColor(0,0,0,50) ; color : white}");

    gameAreaScene->addWidget(&gameStatusLabel);
    ui->pushButton_restartTheGame->setGeometry(controlPanelWidth+gap+(gap+tileEdgeLength)*(boardEdgeSize/2.0-0.75),
                                               gap+(gap+tileEdgeLength)*(boardEdgeSize/2.0+0.8),
                                               gap+tileEdgeLength*1.5,
                                               40);
    ui->pushButton_pause->setEnabled(false);
    ui->pushButton_quiet->setEnabled(false);
}


void GameView::on_pushButton_IDontWantToPlay_clicked()
{
    ui->pushButton_pause->setIcon(QIcon(":/images/resource/skeleton.jpg"));
    ui->pushButton_quiet->setIcon(QIcon(":/images/resource/skeleton.jpg"));
    ui->pushButton_pause->setEnabled(false);
    ui->pushButton_quiet->setEnabled(false);

    ui->pushButton_IDontWantToPlay->setIcon(QIcon(":/images/resource/skeleton.jpg"));

    gameOver();
}

void GameView::on_pushButton_restartTheGame_clicked()
{
    w->sound->soundPlay_stop();
    w->on_pushButton_gameRestart_clicked();
}

void GameView::on_pushButton_goBackToMenu_clicked()
{
    QMessageBox::StandardButton reply;
    w->sound->soundPlay(w->sound->messageAlert);
    reply = QMessageBox::question(this,"Return to Menu","Do you really want to return to the Menu?\n"
                                  "Current game status would be DISCARDED!!",
                          QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes)
        this->close();
}

void GameView::on_pushButton_quiet_clicked()
{
    if(isBackgroundMusicOn)
    {
        w->sound->soundBgMusicPlay_stop();
        ui->pushButton_quiet->setIcon(QIcon(":/images/resource/speakerQuiet.png"));
        ui->pushButton_quiet->setIconSize(QSize(30,30));
        isBackgroundMusicOn = false;
    }
    else    //background music is off
    {
        if(!isGamePaused)   //Resume music when game is in playing status
        {
            if(timeLeft<=20)
                w->sound->soundBgMusicPlay(w->sound->gameviewBackgroundMusicHurry,34500);
            else
                w->sound->soundBgMusicPlay(w->sound->gameviewBackgroundMusic,76800);
        }
        ui->pushButton_quiet->setIcon(QIcon(":/images/resource/speaker.png"));
        ui->pushButton_quiet->setIconSize(QSize(30,30));
        isBackgroundMusicOn = true;
    }

}

void GameView::on_pushButton_pause_clicked()
{
    if(isGamePaused)    //game is in paused status
    {
        timerForStopWatch->start(50);
        keyEventBlock = false;
        //set game status label
        gameStatusLabel.setText(" ");
        gameStatusLabel.setStyleSheet("QLabel{background-color : transparent ; color : transparent}");

        //set sound properties
        if(isBackgroundMusicOn) //Resume music only when background music is on
        {
            if(timeLeft<=20)
                w->sound->soundBgMusicPlay(w->sound->gameviewBackgroundMusicHurry,34500);
            else
                w->sound->soundBgMusicPlay(w->sound->gameviewBackgroundMusic,76800);
        }
        ui->pushButton_pause->setIcon(QIcon(":/images/resource/pause.png"));
        ui->pushButton_pause->setIconSize(QSize(30,30));
        isGamePaused = false;
    }
    else    //game is in playing status
    {
        w->sound->soundPlay(w->sound->pauseAlert);
        timerForStopWatch->stop();
        keyEventBlock = true;

        //set game status label
        gameStatusLabel.setText("Pause\n（￣￢￣）. z Z ");
        QFont font;
        font.setPointSize(28);
        font.setBold(true);
        gameStatusLabel.setFont(font);
        gameStatusLabel.setStyleSheet("QLabel{background-color : QColor(0,0,0,60) ; color : yellow}");
        gameAreaScene->addWidget(&gameStatusLabel);

        //set sound properties
        w->sound->soundBgMusicPlay_stop();
        ui->pushButton_pause->setIcon(QIcon(":/images/resource/play.png"));
        ui->pushButton_pause->setIconSize(QSize(30,30));
        isGamePaused = true;
    }

}
