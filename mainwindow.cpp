
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

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionCopyright_Information_triggered()
{
    copyrightInformation *cpinfo = new copyrightInformation;
    cpinfo->show();

}

void MainWindow::on_actionSettings_triggered()
{
    settings *set = new settings;
    set->show();

}

void MainWindow::on_actionQuit_the_game_triggered()
{
    this->close();

}

void MainWindow::on_actionWhat_is_2048_triggered()
{
    whatis2048 *wh2048 = new whatis2048;
    wh2048->show();
}

void MainWindow::on_actionHow_to_play_triggered()
{
    howtoplay *htplay = new howtoplay;
    htplay->show();
}
