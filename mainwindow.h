#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include "cusbutton.h"
#include "settings.h"
#include "copyrightinformation.h"
#include "whatis2048.h"
#include "howtoplay.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:



public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionCopyright_Information_triggered();

    void on_actionSettings_triggered();

    void on_actionQuit_the_game_triggered();

    void on_actionWhat_is_2048_triggered();

    void on_actionHow_to_play_triggered();

    void on_pushButton_skip_clicked();

    void welcome_form();

    void set_menuBar_enable();

    void on_actionNew_Game_2_triggered();

private:
    QTimer *timer;
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    CusButton *mybutton;

    int settings_goal,settings_boardSizeIndex;
};

#endif // MAINWINDOW_H
