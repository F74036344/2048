#include <QMessageBox>
#include "howtoplay.h"
#include "ui_howtoplay.h"

howtoplay::howtoplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::howtoplay)
{
    ui->setupUi(this);
}

howtoplay::~howtoplay()
{
    delete ui;
}

void howtoplay::on_pushButton_dKnow_clicked()
{
    QMessageBox::information(this,"(￣▽￣)~*","Cheer up!");

}
