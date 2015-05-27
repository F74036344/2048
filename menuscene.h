#ifndef MENUSCENE_H
#define MENUSCENE_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QPushButton>


class MenuScene : public QGraphicsScene
{
public:
    MenuScene();
    ~MenuScene();
    QGraphicsScene *scene;
    QGraphicsView *view;
    void setupAllButton();
        void setupButtonGameStart();
        void setupButtonSettings();
        void setupButtonQuitGame();
private:
    QPushButton *buttonGameStart;
    QPushButton *buttonSettings;
    QPushButton *buttonQuitGame;
};

#endif // MENUSCENE_H
