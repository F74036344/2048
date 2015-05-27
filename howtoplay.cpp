#include <QMessageBox>
#include "howtoplay.h"
#include "ui_howtoplay.h"
#include "mainwindow.h"

extern MainWindow *w;

howtoplay::howtoplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::howtoplay)
{
    ui->setupUi(this);
    QPixmap pic_castle(":/images/resource/castle_s.png");
    ui->label_pic->setPixmap(pic_castle);
    this->setFixedSize(400,300);
}

howtoplay::~howtoplay()
{
    delete ui;
}

void howtoplay::on_pushButton_dKnow_clicked()
{
    w->sound->soundPlay(w->sound->messageAlert);
    QMessageBox::information(this,"(\"☉_☉)","இдஇ");
}




