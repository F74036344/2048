#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>

namespace Ui {
class settings;
}

class settings : public QWidget
{
    Q_OBJECT

public:
    explicit settings(QWidget *parent = 0);
    ~settings();
    int settings_goal,settings_boardSizeIndex;


private slots:
    void on_pushButton_Cancel_clicked();


    void on_pushButton_Ok_clicked();

private:
    Ui::settings *ui;
};

#endif // SETTINGS_H
