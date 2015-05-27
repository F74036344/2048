#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QPushButton>

#include "menuscene.h"



MenuScene::MenuScene()
{
    //Initialize
    scene = new QGraphicsScene;
    buttonGameStart = new QPushButton;
    buttonSettings = new QPushButton;
    buttonQuitGame = new QPushButton;
    //setup buttons
    setupAllButton();
    //add buttons(widgets) into scene
    scene->addWidget(buttonGameStart);
    scene->addWidget(buttonSettings);
    scene->addWidget(buttonQuitGame);
    //Initialize view based on scene
    view = new QGraphicsView(scene);

    //make the view not have the scrollbar
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //set range of scene and view
    scene->setSceneRect(0,0,500,400);
    view->setFixedSize(500,400);
    //show the view
    view->show();
}

MenuScene::~MenuScene()
{

}

void MenuScene::setupAllButton()
{
    setupButtonGameStart();
    setupButtonSettings();
    setupButtonQuitGame();
}



void MenuScene::setupButtonGameStart()
{
    buttonGameStart->setGeometry(QRect(220,200,60,20));
    buttonGameStart->setText("Game START!");
}

void MenuScene::setupButtonSettings()
{
    buttonSettings->setGeometry(QRect(220,275,60,20));
    buttonSettings->setText("Settings");
}

void MenuScene::setupButtonQuitGame()
{
    buttonQuitGame->setGeometry(QRect(220,350,60,20));
    buttonQuitGame->setText("Quit the Game");
}





