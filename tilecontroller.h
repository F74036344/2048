#ifndef TILECONTROLLER_H
#define TILECONTROLLER_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QKeyEvent>

#include "tile.h"

class TileController : public QGraphicsItem
{
public:
    TileController();
    ~TileController();
    void initializeLocFilled(bool);
    void initialzeTileEnabled(bool);

    void generateTile();
    void moveTile(QString);
    void calculateTileValue();
    void removeTile(Tile*);

    void setLocFilled(int,bool);
    bool isLocFilled(int);

private:
    bool *locFilled;
    Tile *tile;

protected:
    void keyPressEvent(QKeyEvent*);


};

#endif // TILECONTROLLER_H
