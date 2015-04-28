#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionCopyright_Information_triggered();

    void on_actionSettings_triggered();

    void on_actionQuit_the_game_triggered();

    void on_actionWhat_is_2048_triggered();

    void on_actionHow_to_play_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H