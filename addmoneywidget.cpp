#include "addmoneywidget.h"
#include "ui_addmoneywidget.h"

addMoneyWidget::addMoneyWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addMoneyWidget)
{
    ui->setupUi(this);
}

addMoneyWidget::~addMoneyWidget()
{
    delete ui;
}

void addMoneyWidget::setCodecData(QMap<QString, DataGP> gpMap)
{
    for(QString str:gpMap.keys()){
        ui->comboxCodec->addItem(str);
    }
}


void addMoneyWidget::on_minecodecBtn_clicked()
{

}

void addMoneyWidget::on_noMineSearchBtn_clicked()
{

}

void addMoneyWidget::on_cancelBtn_clicked()
{
    close();
}

void addMoneyWidget::on_setBtn_clicked()
{
    close();
}
