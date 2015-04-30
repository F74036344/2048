#ifndef COPYRIGHTINFORMATION_H
#define COPYRIGHTINFORMATION_H

#include <QtCore>
#include <QtGui>
#include <QWidget>
#include <QDebug>

namespace Ui {
class copyrightInformation;
}

class copyrightInformation : public QWidget
{
    Q_OBJECT

public:
    explicit copyrightInformation(QWidget *parent = 0);
    ~copyrightInformation();


private:
    Ui::copyrightInformation *ui;

};

#endif // COPYRIGHTINFORMATION_H
