#include <QtGui>
#include <QtCore>
#include <QMessageBox>
#include <QDialog>
#include <QMovie>           //Used to hold video data
#include <QLabel>           //Used to display videos or pictures
#include <QMediaPlayer>     //Used to play sound

//include windows
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"
#include "copyrightinformation.h"
#include "whatis2048.h"
#include "howtoplay.h"
#include "gameview.h"

//include sources
#include "sound.h"
#include "data.h"

extern MainWindow *w; //For interactive acess of classes

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Pin the size of the mainwindow
    this->setFixedSize(320,280);

    //Initialize gameview
    gameview = NULL;


    //Initialize colorCnt;
    colorCnt = new int[4];
    for(int i=0;i<4;i++) colorCnt[i] = 0;

    //Initialize sound object
    sound = new Sound;

    //Initialize data object
    data = new Data;

    //Create timers for later use
    timer = new QTimer;

    //play welcome animation
    QMovie *welcomeAnimation = new QMovie(QString(":/animation/resource/powered_by_Qt.gif"));
    ui->menuBar->setEnabled(false);
    ui->label_displayBeginAnimation->raise();
    ui->pushButton_skip->raise();
    ui->label_displayBeginAnimation->setMovie(welcomeAnimation);
    welcomeAnimation->start();

    //set attributes Qt::DeleteOnClose to every widgets in the welcome scene.
    ui->pushButton_skip->setAttribute(Qt::WA_DeleteOnClose);
    ui->label_displayBeginAnimation->setAttribute(Qt::WA_DeleteOnClose);

    timer->start(9500);
    //When timeout, close the timer and delete the widgets in welcome scene.
    connect(timer,SIGNAL(timeout()),ui->label_displayBeginAnimation,SLOT(close()));
    connect(timer,SIGNAL(timeout()),ui->pushButton_skip,SLOT(close()));
    connect(timer,SIGNAL(timeout()),welcomeAnimation,SLOT(deleteLater()));
    //When timeout, enable the menuBar
    connect(timer,SIGNAL(timeout()),this,SLOT(set_menuBar_enable()));
    //When timeout, stop the timer to prevent timeout signal emits continuous.
    connect(timer,SIGNAL(timeout()),timer,SLOT(stop()));

    //When pushButton_skip is destroyed , execute the welcome_form() to show welcome message.
    connect(ui->pushButton_skip,SIGNAL(destroyed()),this,SLOT(welcome_form()));


}


MainWindow::~MainWindow()
{
    delete[] colorCnt;
    delete ui;
}


void MainWindow::welcome_form()
{
    //Execute the menuscene to show the menu.
    sound->soundPlay(sound->welcomeMusic);
    QMessageBox::information(this,"Welcome","Welcome to 2048~");
}

void MainWindow::set_menuBar_enable()
{
     ui->menuBar->setEnabled(true);
}






//slots function START
void MainWindow::on_actionCopyright_Information_triggered()
{
    copyrightInformation *cpinfo = new copyrightInformation;

    //setWindowModality to prevent the mainwindow from being closed
    cpinfo->setWindowModality(Qt::ApplicationModal);
    cpinfo->show();
    cpinfo->setAttribute(Qt::WA_DeleteOnClose);
    //set mainwindow hide
}

void MainWindow::on_actionSettings_triggered()
{
    settings *set = new settings;

    //setWindowModality to prevent the mainwindow from being closed
    set->setWindowModality(Qt::ApplicationModal);
    set->show();
    set->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::on_actionQuit_the_game_triggered()
{
    QMessageBox::StandardButton reply;
    sound->soundPlay(sound->warningAlert);
    reply = QMessageBox::question(this,"Quit request","Do you really want to quit?",
                          QMessageBox::Yes | QMessageBox::No);
    if(reply==QMessageBox::Yes)
        delete this;

}


void MainWindow::on_actionWhat_is_2048_triggered()
{
    whatis2048 *wh2048 = new whatis2048;

    //setWindowModality to prevent the mainwindow from being closed
    wh2048->setWindowModality(Qt::ApplicationModal);
    wh2048->show();
    wh2048->setAttribute(Qt::WA_DeleteOnClose);
    connect(wh2048,SIGNAL(destroyed()),w,SLOT(show()));
}

void MainWindow::on_actionHow_to_play_triggered()
{
    howtoplay *htplay = new howtoplay;

    //setWindowModality to prevent the mainwindow from being closed
    htplay->setWindowModality(Qt::ApplicationModal);
    htplay->show();
    htplay->setAttribute(Qt::WA_DeleteOnClose);

}
void MainWindow::on_pushButton_skip_clicked()
{
    ui->label_displayBeginAnimation->close();
    ui->pushButton_skip->close();
    set_menuBar_enable();
}

void MainWindow::on_actionNew_Game_2_triggered()
{
    QMessageBox::StandardButton reply;
    sound->soundPlay(sound->warningAlert);
    reply = QMessageBox::question(this,
                                  "Restart",
                                  "Reopen the Game ?",
                          QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes)
    {
         MainWindow *wtmp;
         wtmp = w;
         w = new MainWindow;
         w->show();
         delete wtmp;
    }
}

void MainWindow::on_pushButton_gameStart_clicked()
{
      gameview = new GameView;
      //setWindowModality to prevent the mainwindow from being closed
      gameview->setWindowModality(Qt::ApplicationModal);
      gameview->show();
      gameview->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::on_pushButton_gameRestart_clicked()
{
      delete gameview;
      gameview = new GameView;
      //setWindowModality to prevent the mainwindow from being closed
      gameview->setWindowModality(Qt::ApplicationModal);
      gameview->show();
      gameview->setAttribute(Qt::WA_DeleteOnClose);
}


void MainWindow::on_pushButton_settings_clicked()
{
    settings *set = new settings;

    //setWindowModality to prevent the mainwindow from being closed
    set->setWindowModality(Qt::ApplicationModal);
    set->show();
    set->setAttribute(Qt::WA_DeleteOnClose);
}



void MainWindow::on_pushButton_quitGame_clicked()
{
    QMessageBox::StandardButton reply;
    sound->soundPlay(sound->warningAlert);
    reply = QMessageBox::question(this,"Quit Request","Do you really want to quit?",
                          QMessageBox::Yes | QMessageBox::No);
    if(reply==QMessageBox::Yes)
        delete this;
}
//slot functions END

void MainWindow::on_pushButton_brushFor2048_2_clicked()
{
    w->sound->soundPlay(w->sound->alertCoin);
    colorCnt[0]++;
    if(colorCnt[0]>=6)
        colorCnt[0] = 0;

    switch(colorCnt[0])
    {
    case 0:
        ui->label_2048_2->setStyleSheet("QLabel{background-color : transparent; color : black}");break;
    case 1:
        ui->label_2048_2->setStyleSheet("QLabel{background-color : transparent; color : red}");break;
    case 2:
        ui->label_2048_2->setStyleSheet("QLabel{background-color : transparent; color : yellow}");break;
    case 3:
        ui->label_2048_2->setStyleSheet("QLabel{background-color : transparent; color : green}");break;
    case 4:
        ui->label_2048_2->setStyleSheet("QLabel{background-color : transparent; color : blue}");break;
    case 5:
        ui->label_2048_2->setStyleSheet("QLabel{background-color : transparent; color : pink}");break;
    }

}

void MainWindow::on_pushButton_brushFor2048_0_clicked()
{
    w->sound->soundPlay(w->sound->alertCoin);
    colorCnt[1]++;
    if(colorCnt[1]>=6)
        colorCnt[1] = 0;

    switch(colorCnt[1])
    {
    case 0:
        ui->label_2048_0->setStyleSheet("QLabel{background-color : transparent; color : black}");break;
    case 1:
        ui->label_2048_0->setStyleSheet("QLabel{background-color : transparent; color : red}");break;
    case 2:
        ui->label_2048_0->setStyleSheet("QLabel{background-color : transparent; color : yellow}");break;
    case 3:
        ui->label_2048_0->setStyleSheet("QLabel{background-color : transparent; color : green}");break;
    case 4:
        ui->label_2048_0->setStyleSheet("QLabel{background-color : transparent; color : blue}");break;
    case 5:
        ui->label_2048_0->setStyleSheet("QLabel{background-color : transparent; color : pink}");break;
    }
}

void MainWindow::on_pushButton_brushFor2048_4_clicked()
{
    w->sound->soundPlay(w->sound->alertCoin);
    colorCnt[2]++;
    if(colorCnt[2]>=6)
        colorCnt[2] = 0;

    switch(colorCnt[2])
    {
    case 0:
        ui->label_2048_4->setStyleSheet("QLabel{background-color : transparent; color : black}");break;
    case 1:
        ui->label_2048_4->setStyleSheet("QLabel{background-color : transparent; color : red}");break;
    case 2:
        ui->label_2048_4->setStyleSheet("QLabel{background-color : transparent; color : yellow}");break;
    case 3:
        ui->label_2048_4->setStyleSheet("QLabel{background-color : transparent; color : green}");break;
    case 4:
        ui->label_2048_4->setStyleSheet("QLabel{background-color : transparent; color : blue}");break;
    case 5:
        ui->label_2048_4->setStyleSheet("QLabel{background-color : transparent; color : pink}");break;
    }
}

void MainWindow::on_pushButton_brushFor2048_8_clicked()
{
    w->sound->soundPlay(w->sound->alertCoin);
    colorCnt[3]++;
    if(colorCnt[3]>=6)
        colorCnt[3] = 0;

    switch(colorCnt[3])
    {
    case 0:
        ui->label_2048_8->setStyleSheet("QLabel{background-color : transparent; color : black}");break;
    case 1:
        ui->label_2048_8->setStyleSheet("QLabel{background-color : transparent; color : red}");break;
    case 2:
        ui->label_2048_8->setStyleSheet("QLabel{background-color : transparent; color : yellow}");break;
    case 3:
        ui->label_2048_8->setStyleSheet("QLabel{background-color : transparent; color : green}");break;
    case 4:
        ui->label_2048_8->setStyleSheet("QLabel{background-color : transparent; color : blue}");break;
    case 5:
        ui->label_2048_8->setStyleSheet("QLabel{background-color : transparent; color : pink}");break;
    }
}
