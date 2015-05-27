#include "tilecontroller.h"
#include "mainwindow.h"
#include "tile.h"


extern MainWindow *w;

TileController::TileController()
{
    tile = new Tile[w->gameview->power(w->data->getBoardEdgeSizeValue(),2)];
    locFilled = new bool[w->gameview->power(w->data->getBoardEdgeSizeValue(),2)];
    tileEnabled = new bool[w->gameview->power(w->data->getBoardEdgeSizeIndexValue(),2)];
    initializeLocFilled(false);
    //tile enabled means that the tile has filled.
}

TileController::~TileController()
{
    delete [] tile;
    delete [] locFilled;
}

void TileController::initializeLocFilled(bool tof)
{
    for(int row=0;row<(w->data->getBoardEdgeSizeValue());row++)
        for(int col=0;col<(w->data->getBoardEdgeSizeValue());col++)
            *(locFilled+col+row*(w->data->getBoardEdgeSizeValue())) = tof;

}

void TileController::generateTile()
{
    int totalBlock = w->gameview->power(w->data->getBoardEdgeSizeValue(),2);
    bool ballot[totalBlock];
    //Initialize ballot
    for(int i=0;i<totalBlock;i++)
        ballot[i]=i;
    int availableIndexAmount = totalBlock;
    int current_index;
    while(1)
    {
        Tile *tmp;
        current_index = qrand()%availableIndexAmount;
        if(!isLocFilled(ballot[current_index]))
        {
            tmp = tile+ballot[current_index];
            (tile+ballot[current_index]) = new Tile;
            delete tmp;
            setLocFilled(ballot[current_index],true);
            break;
        }
        else
        {
            for(int k=current_index;k<totalBlock-1;k++)
                ballot[k]=ballot[k+1];
            availableIndexAmount--;
            if(availableIndexAmount<=0)
            {
                w->gameview->gameOver();
                break;
            }
        }
    }
}

void TileController::setLocFilled(int num, bool isfilled)
{
    *(locFilled+col+row*(w->data->getBoardEdgeSizeValue())) = isfilled;
}

bool TileController::isLocFilled(int num)
{
    return *(locFilled+col+row*(w->data->getBoardEdgeSizeValue()));
}

void TileController::setTileEnabled(int num, bool)
{

}

bool TileController::isTileEnabled(int)
{

}


void TileController::moveTile(QString motion)
{
    if(motion==QString("Left"))
        ;
    else if(motion==QString("Right"))
        ;
    else if(motion==QString("Up"))
        ;
    else if(motion==QString("Down"))
        ;

}

void TileController::calculateTileValue()
{

}

void TileController::removeTile(Tile *)
{

}

void TileController::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Left  || event->key() == Qt::Key_A)
       moveTile(QString("Left"));
    else if(event->key() == Qt::Key_Right || event->key() == Qt::Key_D)
       moveTile(QString("Right"));
    else if(event->key() == Qt::Key_Up || event->key() == Qt::Key_W)
        moveTile(QString("Up"));
    else if(event->key() == Qt::Key_Down || event->key() == Qt::Key_S)
        moveTile(QString("Down"));

}





