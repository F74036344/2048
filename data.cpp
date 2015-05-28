#include "data.h"
#include <QTime>

Data::Data()
{
    //set default value
    setBoardEdgeSizeIndexValue(0);
    setWhichGoalButtonChecked(5);
    setTileGenerateAmountIndex(0);
    setWhichTileColorButtonChecked(QString("default"));
    setTimeLimitIsChecked(false);
    setTimeLimitValue(300);
}

Data::~Data()
{

}

//get value functions
int Data::getGoalValue()
{
    return power(2,getWhichGoalButtonChecked()+6);
}
int Data::getWhichGoalButtonChecked()
{
    return whichGoalButtonChecked;
}



int Data::getBoardEdgeSizeValue()
{
    return boardEdgeSizeIndex+4;
}
int Data::getBoardEdgeSizeIndexValue()
{
    return boardEdgeSizeIndex;
}



int Data::getTileGenerateAmount()
{
    return tileGenerateAmountIndex+1;
}
int Data::getTileGenerateAmountIndex()
{
    return tileGenerateAmountIndex;
}

bool Data::isTimeLimitChecked()
{
    return timeLimitIsChecked;
}

int Data::getTimeLimitValue()
{
        return timeLimitValue;
}

QString Data::getWhichTileColorButtonChecked()
{
    return whichTileColorButtonChecked;
}

QColor Data::getTileColor()
{
    if(getWhichTileColorButtonChecked() == QString("random"))
    {
        int tileColorBallot;
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime())); //Initialize qrand()
        tileColorBallot = qrand() % 7;  //Because we have 7 colors:default(white), black, red, yellow, green, blue, purple
        switch(tileColorBallot)
        {
            case 0: //default color(white)
                tileColorIsWhite = true;
                return QColor(Qt::white);
            case 1: //red
                tileColorIsWhite = false;
                return QColor(Qt::red);
            case 2: //yellow
                tileColorIsWhite = false;
                return QColor(0xff,0xcc,0x22);
            case 3: //green
                tileColorIsWhite = false;
                return QColor(0x00,0x88,0x00);
            case 4: //blue
                tileColorIsWhite = false;
                return QColor(Qt::blue);
            case 5: //purple
                tileColorIsWhite = false;
                return QColor(0x77,0x00,0xbb);
            case 6:
                tileColorIsWhite = false;
                return QColor(Qt::black);
        }
    }
    else if(getWhichTileColorButtonChecked() == QString("default"))
    {
        tileColorIsWhite = true;
        return QColor(Qt::white);
    }
    else
    {
        tileColorIsWhite = false;
        if(getWhichTileColorButtonChecked() == QString("black"))
            return QColor(Qt::black);
        else if(getWhichTileColorButtonChecked() == QString("red"))
            return QColor(Qt::red);
        else if(getWhichTileColorButtonChecked() == QString("yellow"))
            return QColor(0xff,0xcc,0x22);    //感覺Qt的yellow太亮，所以自己上網找的個比較不黃的XD
        else if(getWhichTileColorButtonChecked() == QString("green"))
            return QColor(0x00,0x88,0x00);      //感覺Qt的green太亮，自己找了個比較不亮的
        else if(getWhichTileColorButtonChecked() == QString("blue"))
            return QColor(Qt::blue);
        else if(getWhichTileColorButtonChecked() == QString("purple"))
            return QColor(0x77,0x00,0xbb);    //Qt has no purple color ~"~, so it needs to be set manually
    }
}

QColor Data::getTileTextColor()
{
    if(tileColorIsWhite)
        return QColor(Qt::black);
    else
        return QColor(Qt::white);
}




//set value function

void Data::setWhichGoalButtonChecked(int value)
{
    whichGoalButtonChecked = value;
}

void Data::setBoardEdgeSizeIndexValue(int value)
{
    boardEdgeSizeIndex = value;
}

void Data::setTileGenerateAmountIndex(int value)
{
    tileGenerateAmountIndex = value;
}



void Data::setWhichTileColorButtonChecked(QString color)
{
    whichTileColorButtonChecked = color;
}

void Data::setTimeLimitIsChecked(bool tof)
{
    timeLimitIsChecked = tof;
}

void Data::setTimeLimitValue(int value)
{
    timeLimitValue = value;
}



int Data::power(int a, int n)
{
    int sum = 1;
    for(int i=0;i<n;i++)
        sum = sum*a;
    return sum;
}









