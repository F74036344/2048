#ifndef DATA_H
#define DATA_H

#include <QColor>
#include <QString>

class Data
{
public:
    Data();
    ~Data();
    int getGoalValue();
    int getBoardEdgeSizeValue();
    int getWhichGoalButtonChecked();
    int getBoardEdgeSizeIndexValue();
    int getTileGenerateAmount();
    int getTileGenerateAmountIndex();
    QString getWhichTileColorButtonChecked();
    QColor getTileColor();
    QColor getTileTextColor();

    void setGoalValue(int);
    void setWhichGoalButtonChecked(int);
    void setBoardEdgeSizeIndexValue(int);
    void setTileGenerateAmountIndex(int);
    void setTileColor(QColor);
    void setTileTextColor(QColor);
    void setWhichTileColorButtonChecked(QString);

    int power(int,int);

private:
    int whichGoalButtonChecked;
    int boardEdgeSizeIndex;
    int tileGenerateAmountIndex;
    QString whichTileColorButtonChecked;
    bool tileColorIsWhite;
};

#endif // DATA_H
