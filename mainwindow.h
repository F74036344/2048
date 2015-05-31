#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QMediaPlayer>

//include windows
#include "settings.h"
#include "copyrightinformation.h"
#include "whatis2048.h"
#include "howtoplay.h"

//include scenes
#include "gameview.h"
//include sources
#include "sound.h"
#include "data.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Sound *sound;
    Data *data;
    GameView *gameview;
    QColor colorGenerator(int index);


public slots:
    void on_actionCopyright_Information_triggered();

    void on_actionSettings_triggered();

    void on_actionQuit_the_game_triggered();

    void on_actionWhat_is_2048_triggered();

    void on_actionHow_to_play_triggered();

    void on_pushButton_skip_clicked();

    void welcome_form();

    void set_menuBar_enable();

    void on_actionNew_Game_2_triggered();

    void on_pushButton_gameStart_clicked();
    void on_pushButton_gameRestart_clicked();

    void on_pushButton_quitGame_clicked();

    void on_pushButton_settings_clicked();

private slots:
    void on_pushButton_brushFor2048_2_clicked();

    void on_pushButton_brushFor2048_0_clicked();

    void on_pushButton_brushFor2048_4_clicked();

    void on_pushButton_brushFor2048_8_clicked();

private:
    QTimer *timer;
    Ui::MainWindow *ui;
    QGraphicsScene *mScene;
    int colorCnt[]; //index:0=2  , index:1 = 0, index:2 = 4, index:3 = 8


};

#endif // MAINWINDOW_H
