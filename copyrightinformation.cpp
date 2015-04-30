
#include "copyrightinformation.h"
#include "ui_copyrightinformation.h"

copyrightInformation::copyrightInformation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::copyrightInformation)
{
    ui->setupUi(this);
}

copyrightInformation::~copyrightInformation()
{
    delete ui;
}

