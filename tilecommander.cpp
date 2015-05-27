#include "tilecommander.h"
#include "mainwindow.h"

#include <QDebug>
#include <QMessageBox>

extern MainWindow *w;
extern QGraphicsScene *gameAreaScene;

TileCommander::TileCommander()
{
    boardEdgeSize = w->data->getBoardEdgeSizeValue();
    goal = w->data->getGoalValue();
    gap = w->gameview->getGapValue();
    tileEdgeLength = w->gameview->getBlockEdgeLengthValue();
    tileEdgeLengthTotal = (gap+tileEdgeLength)*boardEdgeSize+gap;

    gameStatusLabel = new QLabel;

    //Initialize tileAvailable
    tileAvailable = new bool[power(boardEdgeSize,2)];
    for(int i=0;i < power(boardEdgeSize,2) ;i++)
        setTileAvailable(i,true);

    //Initialize tileCommanderRect
    this->setFlag(QGraphicsItem::ItemIsFocusable);
    this->setFocus();

    //Initialize tile
    //To ensure that the tiles(addresses) are continuous,
    //I use 2nd order pointers
    tile = new Tile* [power(boardEdgeSize,2)];


    //Initialize tile color
    setTileColor(w->data->getTileColor());

    generateTile();

}

TileCommander::~TileCommander()
{
    for(int i=0;i<power(boardEdgeSize,2);i++)   //delete *tile
    {
        delete *(tile+i);
        *(tile+i) = NULL;
    }
    for(int i=0;i<power(boardEdgeSize,2);i++)   //delete tile
    {
        delete (tile+i);
    }
    delete [] tileAvailable;
    delete tmpTile;
    delete gameStatusLabel;
}



void TileCommander::keyPressEvent(QKeyEvent* event)	//step1
{
    //You have already known what this is
    if(event->key()==Qt::Key_Up||event->key()==Qt::Key_W)
    {
        moveTile(QString("Up"));
    }
    else if(event->key()==Qt::Key_Down||event->key()==Qt::Key_S)
    {
        moveTile(QString("Down"));
    }
    else if(event->key()==Qt::Key_Left||event->key()==Qt::Key_A)
    {
        moveTile(QString("Left"));
    }
    else if(event->key()==Qt::Key_Right||event->key()==Qt::Key_D)
    {
        moveTile(QString("Right"));
    }
}



void TileCommander::moveTile(QString motion)	//step2
{
    bool allTileAnimationIsDone=true;
    //第一階段:檢查是否仍有在移動的tile
    for(int i=0;i<power(boardEdgeSize,2);i++)
        if(isTileAvailable(i)==false)
            if((*(tile+i))->isTileActing()==true)
                allTileAnimationIsDone=false;

    if(allTileAnimationIsDone)	//如果每個*tile的animation皆已完成，則進行下一階段
    {
            //開始移動*tile(***大工程要開始了***)
            //*tile不能移動的情況:
            //1.欲移動到的位置超出範圍
            //2.欲移動到的位置有**tile且該**tile(1)不等於自己的值(2)不能移動
            //其他情況就是可以移動的情況了
            int stepToMove;
            if(motion==QString("Up"))	//全部可向上移動的*tile皆向上移動
            {
                //由上而下檢查每一個*tile(這樣下面的*tile往上遇到的*tile就都會是不能移動的了)

                for(currentRow=0;currentRow<boardEdgeSize;currentRow++)
                    for(currentCol=0;currentCol<boardEdgeSize;currentCol++)
                    {
                        if((currentRow-1)<0)	//如果往上一格會超出範圍
                            ;	//沒有任何動作，即此*tile不做移動
                        else
                        {
                            stepToMove=0;
                            bool collisionAnimationNeeded=false;
                            for(nextRow=currentRow-1;nextRow>=0;nextRow--)    //往上檢查每一個*tile
                            {
                                if(isTileAvailable(currentCol+nextRow*boardEdgeSize))
                                    //如果*tile是空的，則stepToMove+1
                                    stepToMove++;
                                else
                                {//如果*tile不是空的，則跳出迴圈

                                    break;
                                }
                            }
                            for(;nextRow>=0;nextRow--)
                            {
                                //再檢查遇到的有**tile的*tile的值是否和自己相同
                                if((*(tile+currentCol+nextRow*boardEdgeSize))->getCurrentValue()==(*(tile+currentCol+currentRow*boardEdgeSize))->getCurrentValue())
                                {    //如果相同，則stepToMove+1並break
                                    stepToMove++;
                                    nextRow--;
                                    break;
                                }

                            }
                            nextRow++;
                            //Started the complicated interaction between *tiles -0-"
                            if(!isTileAvailable(currentCol+nextRow*boardEdgeSize))
                                //If tile is unavailable, then it needs another pointer to store the **tile of the next *tile
                                tmpTile = *(tile+currentCol+nextRow*boardEdgeSize);
                            (*(tile+currentCol+nextRow*boardEdgeSize)) = (*(tile+currentCol+currentRow*boardEdgeSize));
                            (*(tile+currentCol+currentRow*boardEdgeSize)) = NULL;   //Set the unused pointer to NULL is safer

                            //Set the receiver unavailable, and the releaser available
                            setTileAvailable(currentCol+nextRow*boardEdgeSize,false);
                            setTileAvailable(currentCol+currentRow*boardEdgeSize,true);
                            if(!isTileAvailable(currentCol+nextRow*boardEdgeSize))
                            {
                                collisionAnimationNeeded = true;
                                connect(*(tile+currentCol+nextRow*boardEdgeSize),SIGNAL(moveIsDone()),this,SLOT(changeToCollision()));
                                (*(tile+currentCol+nextRow*boardEdgeSize))->move(motion,stepToMove,collisionAnimationNeeded);
                                //move and make collision
                            }
                            else
                            {
                                (*(tile+currentCol+nextRow*boardEdgeSize))->move(motion,stepToMove,collisionAnimationNeeded);
                                //just move
                            }
                        }
                    }
            }
            else if(motion==QString("Down")) //全部可向下移動的*tile皆向下移動
            {
                //由下而上檢查每一個*tile(這樣上面的*tile往下遇到的*tile就都會是不能移動的了)

                for(currentRow=boardEdgeSize-1;currentRow>=0;currentRow--)
                    for(currentCol=0;currentCol<boardEdgeSize;currentCol++)
                    {
                        if((currentRow+1)>=boardEdgeSize)	//如果往下一格會超出範圍
                            ;	//沒有任何動作，即此*tile不做移動
                        else
                        {
                            stepToMove=0;
                            bool collisionAnimationNeeded=false;
                            for(nextRow=currentRow+1;nextRow<boardEdgeSize;nextRow++)    //往下檢查每一個*tile
                            {
                                if(isTileAvailable(currentCol+nextRow*boardEdgeSize))
                                    //如果*tile是空的，則stepToMove+1
                                    stepToMove++;
                                else
                                {//如果*tile不是空的，則跳出迴圈

                                    break;
                                }
                            }
                            for(;nextRow<boardEdgeSize;nextRow++)
                            {
                                //再檢查遇到的有**tile的*tile的值是否和自己相同
                                if((*(tile+currentCol+nextRow*boardEdgeSize))->getCurrentValue()==(*(tile+currentCol+currentRow*boardEdgeSize))->getCurrentValue())
                                {    //如果相同，則stepToMove+1並break
                                    stepToMove++;
                                    nextRow++;
                                    break;
                                }

                            }
                            nextRow--;
                            //Started the complicated interaction between *tiles -0-"
                            if(!isTileAvailable(currentCol+nextRow*boardEdgeSize))
                                //If tile is unavailable, then it needs another pointer to store the **tile of the *nexttile
                                tmpTile = *(tile+currentCol+nextRow*boardEdgeSize);
                            (*(tile+currentCol+nextRow*boardEdgeSize)) = (*(tile+currentCol+currentRow*boardEdgeSize));
                            (*(tile+currentCol+currentRow*boardEdgeSize)) = NULL;   //Set the unused pointer to NULL is safer

                            //Set the receiver unavailable, and the releaser available
                            setTileAvailable(currentCol+nextRow*boardEdgeSize,false);
                            setTileAvailable(currentCol+currentRow*boardEdgeSize,true);
                            if(!isTileAvailable(currentCol+nextRow*boardEdgeSize))
                            {
                                collisionAnimationNeeded = true;
                                connect(*(tile+currentCol+nextRow*boardEdgeSize),SIGNAL(moveIsDone()),this,SLOT(changeToCollision()));
                                (*(tile+currentCol+nextRow*boardEdgeSize))->move(motion,stepToMove,collisionAnimationNeeded);
                                //move and make collision
                            }
                            else
                            {
                                (*(tile+currentCol+nextRow*boardEdgeSize))->move(motion,stepToMove,collisionAnimationNeeded);
                                //just move
                            }
                        }
                    }
            }
            else if(motion==QString("Left")) //全部可向左移動的*tile皆向左移動
            {
                //由左而右檢查每一個*tile(這樣右邊的*tile往左遇到的*tile就都會是不能移動的了)

                for(currentRow=0;currentRow<boardEdgeSize;currentRow++)
                    for(currentCol=0;currentCol<boardEdgeSize;currentCol++)
                    {
                        if((currentCol-1)<0)	//如果往左一格會超出範圍
                            ;	//沒有任何動作，即此*tile不做移動
                        else
                        {
                            stepToMove=0;
                            bool collisionAnimationNeeded=false;
                            for(nextCol=currentCol-1;nextCol>=0;nextCol--)    //往左檢查每一個*tile
                            {
                                if(isTileAvailable(nextCol+currentRow*boardEdgeSize))
                                    //如果*tile是空的，則stepToMove+1
                                    stepToMove++;
                                else
                                {//如果*tile不是空的，則跳出迴圈

                                    break;
                                }
                            }
                            for(;nextCol>=0;nextCol--)
                            {
                                //再檢查遇到的有**tile的*tile的值是否和自己相同
                                if((*(tile+nextCol+currentRow*boardEdgeSize))->getCurrentValue()==(*(tile+currentCol+currentRow*boardEdgeSize))->getCurrentValue())
                                {    //如果相同，則stepToMove+1並break
                                    stepToMove++;
                                    nextCol--;
                                    break;
                                }

                            }
                            nextCol++;
                            //Started the complicated interaction between *tiles -0-"
                            if(!isTileAvailable(nextCol+nextRow*boardEdgeSize))
                                //If tile is unavailable, then it needs another pointer to store the **tile of the *nexttile
                                    tmpTile = *(tile+nextCol+nextRow*boardEdgeSize);
                            (*(tile+nextCol+currentRow*boardEdgeSize)) = (*(tile+currentCol+currentRow*boardEdgeSize));
                            (*(tile+currentCol+currentRow*boardEdgeSize)) = NULL;   //Set the unused pointer to NULL is safer

                            //Set the receiver unavailable, and the releaser available
                            setTileAvailable(nextCol+currentRow*boardEdgeSize,false);
                            setTileAvailable(currentCol+currentRow*boardEdgeSize,true);
                            if(!isTileAvailable(nextCol+currentRow*boardEdgeSize))
                            {
                                collisionAnimationNeeded = true;
                                connect(*(tile+nextCol+currentRow*boardEdgeSize),SIGNAL(moveIsDone()),this,SLOT(changeToCollision()));
                                (*(tile+nextCol+currentRow*boardEdgeSize))->move(motion,stepToMove,collisionAnimationNeeded);
                                //move and make collision
                            }
                            else
                            {
                                (*(tile+nextCol+currentRow*boardEdgeSize))->move(motion,stepToMove,collisionAnimationNeeded);
                                //just move
                            }
                        }
                    }
            }
            else if(motion==QString("Right")) //全部可向右移動的*tile皆向右移動
            {
                //由右而左檢查每一個*tile(這樣左邊的*tile往右遇到的*tile就都會是不能移動的了)

                for(currentRow=0;currentRow<boardEdgeSize;currentRow++)
                    for(currentCol=boardEdgeSize-1;currentCol>=0;currentCol--)
                    {
                        if(currentCol>=boardEdgeSize)	//如果往右一格會超出範圍
                            ;	//沒有任何動作，即此*tile不做移動
                        else
                        {
                            stepToMove=0;
                            bool collisionAnimationNeeded=false;
                            for(nextCol=0;nextCol<currentCol;nextCol++)    //往右檢查每一個*tile
                            {
                                if(isTileAvailable(nextCol+currentRow*boardEdgeSize))
                                    //如果*tile是空的，則stepToMove+1
                                    stepToMove++;
                                else
                                {//如果*tile不是空的，則跳出迴圈

                                    break;
                                }
                            }
                            for(;nextCol<boardEdgeSize;nextCol++)
                            {
                                //再檢查遇到的有**tile的*tile的值是否和自己相同
                                if((*(tile+nextCol+currentRow*boardEdgeSize))->getCurrentValue()==(*(tile+currentCol+currentRow*boardEdgeSize))->getCurrentValue())
                                {    //如果相同，則stepToMove+1並break
                                    stepToMove++;
                                    nextCol++;
                                    break;
                                }

                            }
                            nextCol--;
                            //Started the complicated interaction between *tiles -0-"
                            if(!isTileAvailable(nextCol+currentRow*boardEdgeSize))
                                //If tile is unavailable, then it needs another pointer to store the **tile of the *nexttile
                                    tmpTile = *(tile+nextCol+currentRow*boardEdgeSize);
                            (*(tile+nextCol+currentRow*boardEdgeSize)) = (*(tile+currentCol+currentRow*boardEdgeSize));
                            (*(tile+currentCol+currentRow*boardEdgeSize)) = NULL;   //Set the unused pointer to NULL is safer

                            //Set the receiver unavailable, and the releaser available
                            setTileAvailable(nextCol+currentRow*boardEdgeSize,false);
                            setTileAvailable(currentCol+currentRow*boardEdgeSize,true);
                            if(!isTileAvailable(nextCol+currentRow*boardEdgeSize))
                            {
                                collisionAnimationNeeded = true;
                                connect(*(tile+nextCol+currentRow*boardEdgeSize),SIGNAL(moveIsDone()),this,SLOT(changeToCollision()));
                                (*(tile+nextCol+currentRow*boardEdgeSize))->move(motion,stepToMove,collisionAnimationNeeded);
                                //move and make collision
                            }
                            else
                            {
                                (*(tile+nextCol+currentRow*boardEdgeSize))->move(motion,stepToMove,collisionAnimationNeeded);
                                //just move
                            }
                        }
                    }
            }

            generateTile();	//進入step3

    }
    else	//如果還有*tile的animation還未完成，則沒有動作並退出function，等待下一次的keyPressEvent
    {;}//do nothing

}

void TileCommander::changeToCollision()
{
    int nextValue = tmpTile->getCurrentValue();
    delete tmpTile;
    tmpTile = NULL;
    int newValue = nextValue+(*(tile+nextCol+nextRow*boardEdgeSize))->getCurrentValue();
    delete (*(tile+nextCol+nextRow*boardEdgeSize));
    (*(tile+nextCol+nextRow*boardEdgeSize)) = new Tile(nextRow,nextCol,newValue);
}



void TileCommander::generateTile()	//step3
{
    int ballot[power(boardEdgeSize,2)];
    int current_index=0;
    //製籤
    for(int i=0;i<power(boardEdgeSize,2);i++)
    {
        if(isTileAvailable(i))
        {
            //將available的*tile加到ballot中
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
        //並傳給新的**tile其位置資訊
        int row = ballot[current_index] / boardEdgeSize; //計算該新**tile在第幾row
        int col = ballot[current_index] % boardEdgeSize; //計算該新**tile在第幾col
        int gap = 5;    int tileEdgeLength = 80;
        (*(tile+ballot[current_index])) = new Tile(row,col,2);  qDebug() << "<tilecommander.cpp>generate new tile No."<<i;
        (*(tile+ballot[current_index]))->setPos(gap + col*(tileEdgeLength+gap) + tileEdgeLength/2,gap + row*(tileEdgeLength+gap) + tileEdgeLength/2);
        if(getTileColor() == QColor(Qt::white))
            (*(tile+ballot[current_index]))->setPen(QPen(Qt::black,3));
        else
            (*(tile+ballot[current_index]))->setPen(QPen(getTileColor(),1));
        (*(tile+ballot[current_index]))->setBrush(getTileColor());
        (*(tile+ballot[current_index]))->label_demoValue->setGeometry(
                    gap + col*(tileEdgeLength+gap),
                    gap + row*(tileEdgeLength+gap)+20,
                    80,40);
        gameAreaScene->addItem((*(tile+ballot[current_index])));
        gameAreaScene->addWidget((*(tile+ballot[current_index]))->label_demoValue);

        //籤已被用走一個->該籤需從ballot中刪掉
        for(int j=current_index;j<total_index_available_amount-1;j++)
            ballot[j] = ballot[j+1];

        //在tile得到新的*tile後，設定其available值為false
        setTileAvailable(ballot[current_index],false);

        //已配置一個新的*tile，so total_index_available_amount -1;
        total_index_available_amount--;
    }
     checkIfAnyTileReachGoal();	//進入step4:檢查有沒有達到goal的tile

}



void TileCommander::checkIfAnyTileReachGoal()	//step4
{
    qDebug() << "goal : "<<goal;
    for(int i=0;i<power(boardEdgeSize,2);i++)
    {

        if(isTileAvailable(i) == false)     //檢查沒有空位(即有tile)的*tile 的tileCurrentValue是否已到達目標
        {
            //if((*(tile+i))->getCurrentValue() >= goal)
            {
               QFont font;
               font.setBold(true);
               font.setPointSize(60);
               gameStatusLabel->setGeometry(0,0,tileEdgeLengthTotal,tileEdgeLengthTotal);
               gameStatusLabel->setText(QString("You Win\n\\^o^/"));
               gameStatusLabel->setStyleSheet("QLabel{background-color : QColor(50,50,50,120) ;color : blue}");
               gameStatusLabel->setFont(font);
               gameStatusLabel->setAlignment(Qt::AlignCenter);
               gameAreaScene->addWidget(gameStatusLabel);
                //gameStatusLabel.setText(QString("Game Over\n(oДo)\""));
                break;

            }
        }
    }
    //checkIfAnyTileIsMovable();	//進入step5:檢查還有沒有可以動的*tile
}


void TileCommander::checkIfAnyTileIsMovable()	//step5
{
         //第一階段開始檢查是否有available的*tile
        bool allTileIsUnavailable=true;
            for(int k=0;k<power(boardEdgeSize,2);k++)
                if(isTileAvailable(k))
                    allTileIsUnavailable=false;
        if(allTileIsUnavailable)	//如果沒有空的*tile，則進入第二階段
        //第二階段:開始檢查是否所有*tile皆已不能動
        {
            bool allTileCanNotMove = true;
            //向右及向下檢查是否有同樣value的**tile存在
            for(int row=0;row<boardEdgeSize;row++)
                for(int col=0;col<boardEdgeSize;col++)
                {
                    //向右檢查
                    if(col+1>=boardEdgeSize)	//如果右邊沒東西就不用檢查
                        ;//do nothing
                    else
                    {
                        //如果有相同value的，就把allTileCanNotMove設為false
                        if((*(tile+col+row*boardEdgeSize))->getCurrentValue()==(*(tile+(col+1)+row*boardEdgeSize))->getCurrentValue())
                            allTileCanNotMove = false;
                    }
                    //向下檢查
                    if(row+1>=boardEdgeSize)	//如果下面沒東西就不用檢查
                        ;//do nothing
                    else
                    {
                        //如果有相同value的，就把allTileCanNotMove設為false
                        if((*(tile+col+row*boardEdgeSize))->getCurrentValue()==(*(tile+col+(row+1)*boardEdgeSize))->getCurrentValue())
                            allTileCanNotMove = false;
                    }

                }

            if(allTileCanNotMove)
            {
                //如果所有*tile皆不能動，則gameOver
                //gameOver();
            }
            //如果還有*tile能動的話，則沒有動作退出此函式，等待下一次的keyPressEvent
    }

}






bool TileCommander::isTileAvailable(int index)
{
    return *(tileAvailable+index);
}

void TileCommander::setTileColor(QColor color)
{
    tileColor = color;
}

QColor TileCommander::getTileColor()
{
    return tileColor;
}





void TileCommander::setTileAvailable(int index,bool tof)
{
    *(tileAvailable+index)=tof;
}





int TileCommander::power(int a,int n)	//a的n平方
{
    return w->gameview->power(a,n); //直接抄寫在gameview裡的power-0-
}



/*QRectF TileCommander::boundingRect() const
{
    int viewEdgeLength = (w->gameview->getGapValue()) + (w->gameview->getBlockEdgeLengthValue());
    return QRectF(0,0,viewEdgeLength,viewEdgeLength);
}
void TileCommander::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QBrush brush(Qt::transparent);
    //painter->fillRect(this,brush);

}
*/


