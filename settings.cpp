#include "settings.h"
#include "mainwindow.h"
#include "ui_settings.h"

extern MainWindow *w;

settings::settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settings)
{
    ui->setupUi(this);
    //Add options into comboBox and comboBox_tileGenerateAmount
    for(int i=0;i<3;i++)
        ui->comboBox->addItem(QString(QString::number(i+4)+"x"+QString::number(i+4)));
    for(int i=0;i<3;i++)
        ui->comboBox_tileGenerateAmount->addItem(QString(QString::number(i+1)));

    //Read comboBox imformation from data
    ui->comboBox->setCurrentIndex(w->data->getBoardEdgeSizeIndexValue());

    //Read comboBox_tileGenerateAmount imformation from data
    ui->comboBox_tileGenerateAmount->setCurrentIndex(w->data->getTileGenerateAmountIndex());

    //Read radioButton imformation(goal information)from data
    if(w->data->getWhichGoalButtonChecked() == 1)
        ui->radioButton_1->setChecked(true);
    else if(w->data->getWhichGoalButtonChecked() == 2)
        ui->radioButton_2->setChecked(true);
    else if(w->data->getWhichGoalButtonChecked() == 3)
        ui->radioButton_3->setChecked(true);
    else if(w->data->getWhichGoalButtonChecked() == 4)
        ui->radioButton_4->setChecked(true);
    else if(w->data->getWhichGoalButtonChecked() == 5)
        ui->radioButton_5->setChecked(true);
    else if(w->data->getWhichGoalButtonChecked() == 6)
        ui->radioButton_6->setChecked(true);

    //Read radioButton imformation(tileColor information)from data
    if(w->data->getWhichTileColorButtonChecked() == QString("default"))
        ui->radioButton_defaultColor->setChecked(true);
    else if(w->data->getWhichTileColorButtonChecked() == QString("random"))
        ui->radioButton_randomColor->setChecked(true);
    else if(w->data->getWhichTileColorButtonChecked() == QString("black"))
        ui->radioButton_black->setChecked(true);
    else if(w->data->getWhichTileColorButtonChecked() == QString("red"))
        ui->radioButton_red->setChecked(true);
    else if(w->data->getWhichTileColorButtonChecked() == QString("yellow"))
        ui->radioButton_yellow->setChecked(true);
    else if(w->data->getWhichTileColorButtonChecked() == QString("green"))
        ui->radioButton_green->setChecked(true);
    else if(w->data->getWhichTileColorButtonChecked() == QString("blue"))
        ui->radioButton_blue->setChecked(true);
    else if(w->data->getWhichTileColorButtonChecked() == QString("purple"))
        ui->radioButton_purple->setChecked(true);
}

settings::~settings()
{
    delete ui;
}

void settings::on_pushButton_Cancel_clicked()
{
    //Save nothing
    this->close();
}

void settings::on_pushButton_Ok_clicked()
{
    //Save boardEdgeSizeIndexValue to data
    w->data->setBoardEdgeSizeIndexValue(ui->comboBox->currentIndex());
    //Save tileGenerateAmountIndex to data
    w->data->setTileGenerateAmountIndex(ui->comboBox_tileGenerateAmount->currentIndex());

    //Save goal information to data
    if(ui->radioButton_1->isChecked())
        w->data->setWhichGoalButtonChecked(1);
    else if(ui->radioButton_2->isChecked())
        w->data->setWhichGoalButtonChecked(2);
    else if(ui->radioButton_3->isChecked())
        w->data->setWhichGoalButtonChecked(3);
    else if(ui->radioButton_4->isChecked())
        w->data->setWhichGoalButtonChecked(4);
    else if(ui->radioButton_5->isChecked())
        w->data->setWhichGoalButtonChecked(5);
    else if(ui->radioButton_6->isChecked())
        w->data->setWhichGoalButtonChecked(6);

    //Save tileColor information to data
    if(ui->radioButton_defaultColor->isChecked())
        w->data->setWhichTileColorButtonChecked(QString("default"));
    else if(ui->radioButton_randomColor->isChecked())
        w->data->setWhichTileColorButtonChecked(QString("random"));
    else if(ui->radioButton_black->isChecked())
        w->data->setWhichTileColorButtonChecked(QString("black"));
    else if(ui->radioButton_red->isChecked())
        w->data->setWhichTileColorButtonChecked(QString("red"));
    else if(ui->radioButton_yellow->isChecked())
        w->data->setWhichTileColorButtonChecked(QString("yellow"));
    else if(ui->radioButton_green->isChecked())
        w->data->setWhichTileColorButtonChecked(QString("green"));
    else if(ui->radioButton_blue->isChecked())
        w->data->setWhichTileColorButtonChecked(QString("blue"));
    else if(ui->radioButton_purple->isChecked())
        w->data->setWhichTileColorButtonChecked(QString("purple"));



    this->close();


}


