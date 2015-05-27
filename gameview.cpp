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

    w->sound->soundBgMusicPlay(w->sound->gameviewBackgroundMusic,76800);

    gameEnd = false;

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
    ui->pushButton_goBackToMenu->setGeometry(8,
                                             gap+(w->data->getBoardEdgeSizeValue()-1)*(tileEdgeLength+gap)+tileEdgeLength/2,
                                             controlPanelWidth-8-8,
                                             30);
    ui->pushButton_IDontWantToPlay->setGeometry(8,
                                             gap+(w->data->getBoardEdgeSizeValue()-2)*(tileEdgeLength+gap)+tileEdgeLength/2+20,
                                             controlPanelWidth-8-8,
                                             50);
    setTileColor(w->data->getTileColor());

    score = 0;
    ui->label_scoreValue->setText(QString::number(score));
    ui->label_scoreValue->setStyleSheet("QLabel{background-color : transparent ; color : blue}");
    ui->label_scoreValue->setAlignment(Qt::AlignCenter);


    ui->label_goalValue->setText(QString::number(w->data->getGoalValue()));
    ui->label_goalValue->setStyleSheet("QLabel{background-color : transparent ; color : red}");
    ui->label_goalValue->setAlignment(Qt::AlignCenter);

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

    gameStatusBackground.setRect(0,0,gameAreaEdgeLength,gameAreaEdgeLength);
    gameStatusBackground.setPos(0,0);
    gameStatusBackground.setBrush(Qt::transparent);
    gameStatusBackground.setPen(QPen(Qt::transparent,0));
    //The two objects above would be added to scene when gameOver() or gameWin() is called

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
    //delete gameAreaScene;

}

void GameView::keyPressEvent(QKeyEvent *event)  //step 1:receive key event from player
{
    //會傳event的情況:  1.尚未gameEnd(gameOver or gameWin)
                    qDebug() << event->key()<<"Key detected";
    if(!gameEnd)    //game尚未結束，則傳event
    {

            if(event->key()==Qt::Key_Up||event->key()==Qt::Key_W)
            {
                emitMoveTileSignalToAll(QString("Up"));
                generateTile();
            }
            else if(event->key()==Qt::Key_Down||event->key()==Qt::Key_S)
            {
                emitMoveTileSignalToAll(QString("Down"));
                generateTile();
            }
            else if(event->key()==Qt::Key_Left||event->key()==Qt::Key_A)
            {
                emitMoveTileSignalToAll(QString("Left"));
                generateTile();
            }
            else if(event->key()==Qt::Key_Right||event->key()==Qt::Key_D)
            {
                emitMoveTileSignalToAll(QString("Right"));
                generateTile();
            }


    }
}
void GameView::emitMoveTileSignalToAll(QString motion)
{
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

}



void GameView::moveTile(int index,QString motion) //step 2:move tiles
{
    if((*(tile+index)) == NULL)
        ;   //Do nothing
    else
    {
        int row,col,currentRow,currentCol;
        int step=0;
        if(motion == QString("Up"))
        {
                    qDebug() << "motion Up received";
            row = index/boardEdgeSize;
            col = index%boardEdgeSize;
            currentRow = row;
            currentCol = col;
            //由下往上檢查
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
                    scoreAddAndShow(*(currentValueOfTile+currentCol+currentRow*boardEdgeSize));
                }
            }

        }
        else if(motion == QString("Down"))
        {
            row = index/boardEdgeSize;
            col = index%boardEdgeSize;
            currentRow = row;
            currentCol = col;
            //由上往下檢查
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
                    scoreAddAndShow(*currentValueOfTile+currentCol+currentRow*boardEdgeSize);
                }
            }

        }
        else if(motion == QString("Left"))
        {
            row = index/boardEdgeSize;
            col = index%boardEdgeSize;
            currentRow = row;
            currentCol = col;
            //由右往左檢查

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
                    scoreAddAndShow(*currentValueOfTile+currentCol+currentRow*boardEdgeSize);
                }
            }

        }
        else if(motion == QString("Right"))
        {
            row = index/boardEdgeSize;
            col = index%boardEdgeSize;
            currentRow = row;
            currentCol = col;
            //由左往右檢查
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
                    scoreAddAndShow(*currentValueOfTile+currentCol+currentRow*boardEdgeSize);
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
    int tileAmountToGenerate=(w->data->getTileGenerateAmount());
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
    //第一階段開始檢查是否有available的*tile
   bool allTileIsUnavailable=true;
       for(int k=0;k<power(boardEdgeSize,2);k++)
           if(*(tile+k) == NULL)
               allTileIsUnavailable=false;
   if(allTileIsUnavailable)	//如果沒有空的*tile，則進入第二階段
   //第二階段:開始檢查是否所有*tile皆已不能動
   {
       bool allTileCanNotMove = true;
       //向右及向下檢查是否有1.Available的tile 2.同樣value的tile存在
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
    }
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
    w->sound->soundBgMusicPlay_stop();
    w->sound->soundPlay(w->sound->gameOverMusic);
    gameEnd = true;
    gameStatusLabel.setText("Game Over\n(oДo)\"");
    QFont font;
    font.setPointSize(28);
    font.setBold(true);
    gameStatusLabel.setFont(font);
    gameStatusLabel.setStyleSheet("QLabel{background-color : QColor(0,0,0,50) ; color : red}");

    gameStatusBackground.setPen(QPen(QColor(30,30,30,50),1));
    gameStatusBackground.setBrush(QColor(30,30,30,50));

    gameAreaScene->addItem(&gameStatusBackground);
    gameAreaScene->addWidget(&gameStatusLabel);

}

void GameView::gameWin()
{
    w->sound->soundBgMusicPlay_stop();
    w->sound->soundPlay(w->sound->gameOverMusic);
    gameEnd = true;
    gameStatusLabel.setText("You Win The Game\n\\^o^/");
    QFont font;
    font.setPointSize(28);
    font.setBold(true);
    gameStatusLabel.setFont(font);
    gameStatusLabel.setStyleSheet("QLabel{background-color : QColor(0,0,0,50) ; color : green}");

    gameAreaScene->addItem(&gameStatusBackground);
    gameAreaScene->addWidget(&gameStatusLabel);
}



void GameView::on_pushButton_goBackToMenu_clicked()
{
    QMessageBox::StandardButton reply;
    w->sound->soundPlay(w->sound->messageAlert);
    reply = QMessageBox::question(this,"Go back to Menu","Do you really want to return to Menu?\n"
                                  "Current game status would be DISCARDED!!",
                          QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes)
        delete this;
}

void GameView::on_pushButton_IDontWantToPlay_clicked()
{
    gameOver();
}
