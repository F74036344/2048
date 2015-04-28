#include "settings.h"
#include "ui_settings.h"

settings::settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settings)
{
    ui->setupUi(this);
    for(int i=4;i<=6;i++)
        ui->comboBox->addItem(QString::number(i)+"x"+QString::number(i)+"("+QString::number(i*i)+")");
}

settings::~settings()
{
    delete ui;
}

void settings::on_pushButton_Cancel_clicked()
{




}
