#include "updatemygpdialog.h"
#include "ui_updatemygpdialog.h"
#include "signalm.h"
#include "stockView/stockcanvas.h"

updateMyGpDialog::updateMyGpDialog(DataHaveGP map, QWidget *parent) :
    QDialog(parent),
    m_map(map),
    ui(new Ui::updateMyGpDialog)
{
    ui->setupUi(this);
    ui->codecLabel->setText(m_map.codec);
    ui->nameLabel->setText(m_map.name);
    ui->totalEdit->setText(QString::number(m_map.payallPrice));
    ui->numEdit->setText(QString::number(m_map.haveNum));
    double eveNum = m_map.payallPrice / m_map.haveNum;
    ui->eveEdit->setText(QString::number(eveNum));
    setWindowTitle("购买股票配置");

    QString codec = m_map.codec.replace("sz", "1");
    codec == codec.replace("sh", "0");
    char  *chSecID;
    QByteArray baSecID = codec.toLatin1(); // must
    chSecID = baSecID.data();
    m_stockWidget = new StockCanvas(codec);
    m_stockWidget->setMinimumSize(400, 300);
    ui->stockView->addWidget(m_stockWidget);
}


updateMyGpDialog::~updateMyGpDialog()
{
    m_stockWidget->deleteLater();
    m_stockWidget = nullptr;
    delete ui;
}

void updateMyGpDialog::on_okBtn_clicked()
{
    //    if(m_db.isValid()){

    //        double dnum=inum;
    //        double addMoney =dnum*chasePrice;
    //        QSqlQuery query(m_db);
    //        //占位符 : + 自定义名字
    //        //        query.prepare("insert into myData(code) values(:code)");
    //        QString insert_sql = "insert into haveData values (?, ?, ?)";
    //        query.prepare(insert_sql);
    //        query.addBindValue(codec);
    //        query.addBindValue(QString::number(addMoney));
    //        query.addBindValue(inum);
    //        //        query.bindValue(":code", str);
    //        query.setForwardOnly(true);
    //        query.exec();
    //        DataHaveGP gp;
    //        gp.codec=codec;
    //        gp.payallPrice=addMoney;
    //        gp.haveNum=inum;
    //        m_mMyGp.insert(codec,gp);
    //    }
    double money = ui->totalEdit->text().toDouble();
    int number = ui->numEdit->text().toInt();
    QString str = QString("UPDATE  haveData SET money = %1 , number= %2  WHERE code ='%3'").arg(money).arg(number).arg(m_map.codec);
    emit signalM::instance()->sendExecDb(str);
    emit signalM::instance()->refreashHaveData();
    close();
}

void updateMyGpDialog::on_cancelBtn_clicked()
{
    close();
}

void updateMyGpDialog::on_totalEdit_textChanged(const QString &arg1)
{
    double eveNum = ui->totalEdit->text().toDouble() / ui->numEdit->text().toDouble();
    QString eveStr = QString::number(eveNum);
    if (eveStr != ui->eveEdit->text()) {
        ui->eveEdit->setText(eveStr);
    }

}

void updateMyGpDialog::on_numEdit_textChanged(const QString &arg1)
{
    double eveNum = ui->totalEdit->text().toDouble() / ui->numEdit->text().toDouble();
    QString eveStr = QString::number(eveNum);
    if (eveStr != ui->eveEdit->text()) {
        ui->eveEdit->setText(eveStr);
    }
}

void updateMyGpDialog::on_eveEdit_textChanged(const QString &arg1)
{
    double totalNum = ui->eveEdit->text().toDouble() * ui->numEdit->text().toInt();
    QString totalStr = QString::number(totalNum);
    if (totalStr != ui->totalEdit->text()) {
        ui->totalEdit->setText(totalStr);
    }
}
