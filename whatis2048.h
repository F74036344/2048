#ifndef WHATIS2048_H
#define WHATIS2048_H

#include <QWidget>

namespace Ui {
class whatis2048;
}

class whatis2048 : public QWidget
{
    Q_OBJECT

public:
    explicit whatis2048(QWidget *parent = 0);
    ~whatis2048();

private slots:
    void on_pushButton_dKnow_clicked();

private:
    Ui::whatis2048 *ui;
};

#endif // WHATIS2048_H
