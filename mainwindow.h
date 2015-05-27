#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QMediaPlayer>

//include windows
#include "cusbutton.h"
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

private:
    QTimer *timer;
    Ui::MainWindow *ui;
    QGraphicsScene *mScene;
    CusButton *mybutton;


};

#endif // MAINWINDOW_H
