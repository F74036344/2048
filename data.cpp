#include "data.h"
#include <QTime>

Data::Data()
{
    //set default value
    setBoardEdgeSizeIndexValue(0);
    setWhichGoalButtonChecked(6);
    setTileGenerateAmountIndex(0);
    setWhichTileColorButtonChecked(QString("random"));  //default color is random
    setTimeLimitIsChecked(false);
    setTimeLimitValue(600);
}

Data::~Data()
{

}

//get value functions
int Data::getGoalValue()
{
    if(getWhichGoalButtonChecked()==8)  //ratioButton_XD
        return 4;
    else
        return power(2,getWhichGoalButtonChecked()+5);
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

QString Data::getTileColor()
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
                return QString("white");
            case 1: //red
                tileColorIsWhite = false;
                return QString("red");
            case 2: //yellow
                tileColorIsWhite = false;
                return QString("yellow");
            case 3: //green
                tileColorIsWhite = false;
                return QString("green");
            case 4: //blue
                tileColorIsWhite = false;
                return QString("blue");
            case 5: //purple
                tileColorIsWhite = false;
                return QString("purple");
            case 6:
                tileColorIsWhite = false;
                return QString("black");
        }
    }
    else if(getWhichTileColorButtonChecked() == QString("white"))
    {
        tileColorIsWhite = true;
        return QString("white");
    }
    else
    {
        tileColorIsWhite = false;
        if(getWhichTileColorButtonChecked() == QString("black"))
            return QString("black");
        else if(getWhichTileColorButtonChecked() == QString("red"))
            return QString("red");
        else if(getWhichTileColorButtonChecked() == QString("yellow"))
            return QString("yellow");
        else if(getWhichTileColorButtonChecked() == QString("green"))
            return QString("green");
        else if(getWhichTileColorButtonChecked() == QString("blue"))
            return QString("blue");
        else if(getWhichTileColorButtonChecked() == QString("purple"))
            return QString("purple");
    }
}

QString Data::getTileTextColor()
{
    if(tileColorIsWhite)
        return QString("black");
    else
        return QString("white");
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









