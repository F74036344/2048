#ifndef HOWTOPLAY_H
#define HOWTOPLAY_H

#include <QWidget>

namespace Ui {
class howtoplay;
}

class howtoplay : public QWidget
{
    Q_OBJECT

public:
    explicit howtoplay(QWidget *parent = 0);
    ~howtoplay();

private slots:
    void on_pushButton_dKnow_clicked();

private:
    Ui::howtoplay *ui;
};

#endif // HOWTOPLAY_H
