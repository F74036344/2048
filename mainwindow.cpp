#include <QtGui>
#include <QtCore>
#include <QMessageBox>
#include <QDialog>
#include <QMovie>
#include <QLabel>


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"
#include "copyrightinformation.h"
#include "whatis2048.h"
#include "howtoplay.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Create a timer for later use
    QTimer *timer=new QTimer;

    //play welcome animation
    QMovie *welcomeAnimation = new QMovie(":/animation/resource/powered_by_Qt.gif");
    ui->menuBar->setEnabled(false);
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
    //When ... , execute the welcome_form() to show welcome message.
    connect(ui->pushButton_skip,SIGNAL(destroyed()),this,SLOT(welcome_form()));

    //When timeout, stop the timer to prevent timeout signal emits continuous.
    connect(timer,SIGNAL(timeout()),timer,SLOT(stop()));


}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::welcome_form()
{
    QMessageBox::information(this,"Welcome","Welcome to 2048~");
}

void MainWindow::set_menuBar_enable()
{
     ui->menuBar->setEnabled(true);
}

void MainWindow::on_actionCopyright_Information_triggered()
{
    copyrightInformation *cpinfo = new copyrightInformation;
    cpinfo->show();
    cpinfo->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::on_actionSettings_triggered()
{
    settings *set = new settings;
    set->show();
    set->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::on_actionQuit_the_game_triggered()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Quit request","Do you really want to quit?",
                          QMessageBox::Yes | QMessageBox::No);
    if(reply==QMessageBox::Yes)
        this->close();

}


void MainWindow::on_actionWhat_is_2048_triggered()
{
    whatis2048 *wh2048 = new whatis2048;
    wh2048->show();
    wh2048->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::on_actionHow_to_play_triggered()
{
    howtoplay *htplay = new howtoplay;
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
    reply = QMessageBox::question(this,
                                  "New Window Request",
                                  "Open a new Game Window?\n"
                                  "Current Window would be CLOSE!!",
                          QMessageBox::Yes | QMessageBox::No);
    if(reply==QMessageBox::Yes)
    {

    }
}
