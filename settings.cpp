#include "settings.h"
#include "mainwindow.h"
#include "ui_settings.h"

settings::settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settings)
{
    ui->setupUi(this);

}

settings::~settings()
{
    delete ui;
}

void settings::on_pushButton_Cancel_clicked()
{

    this->close();
}

void settings::on_pushButton_Ok_clicked()
{

        this->close();
}


