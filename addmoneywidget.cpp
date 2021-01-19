#include "addmoneywidget.h"
#include "ui_addmoneywidget.h"
#include "application.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTextCodec>

addMoneyWidget::addMoneyWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addMoneyWidget)
{
    ui->setupUi(this);
    //创建一个管理器
    manager = new QNetworkAccessManager(this);
    //   reply = manager->get(request);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
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
    App->m_db;
    if(!ui->comboxCodec->currentText().isEmpty()){
        QString bumStr="http://hq.sinajs.cn/list="+ui->comboxCodec->currentText();
        QNetworkRequest request;
        request.setUrl(QUrl(bumStr));
        reply=manager->get(request);
    }
}

void addMoneyWidget::addGP()
{
    if(App->m_db.isValid()){
        int inum=ui->editNum->text().toInt();
        double dnum=ui->editNum->text().toDouble();
        double chasePrice=ui->editPurchasePrice->text().toDouble();
        double addMoney=dnum*chasePrice;
        QSqlQuery query(App->m_db);
        //占位符 : + 自定义名字
//        query.prepare("insert into myData(code) values(:code)");
        QString insert_sql = "insert into haveData values (?, ?, ?)";
        query.prepare(insert_sql);
        query.addBindValue(currentData.codec);
        query.addBindValue(QString::number(addMoney));
        query.addBindValue(inum);
//        query.bindValue(":code", str);
        query.setForwardOnly(true);
        query.exec();
    }
}

void addMoneyWidget::on_noMineSearchBtn_clicked()
{
    QString ZQ;
    if(ui->comboxJYS->currentText().contains("上")){
        ZQ="sh";
    }
    else if(ui->comboxJYS->currentText().contains("深")){
        ZQ="sz";
    }
    if(!ui->editCodec->text().isEmpty()){
        QString num=ZQ+ui->editCodec->text();
        QString bumStr="http://hq.sinajs.cn/list="+num;
        QNetworkRequest request;
        request.setUrl(QUrl(bumStr));
        reply=manager->get(request);
    }
}
void addMoneyWidget::replyFinished(QNetworkReply* reply)
{
    QByteArray data = reply->readAll();
    QTextCodec *tc = QTextCodec::codecForName("GBK");

    QString str = tc->toUnicode(data);//str如果是中文则是中文字符
    str.remove("\n");

    QList<QString> numList=str.split(";");
    for(QString numStr:numList)
    {
        if(!numStr.isEmpty()){
            QList<QString> strList=numStr.split(",");
            if(strList.count()>31){
                QStringList str1=strList.at(0).split("str_");
                if(str1.count()==2){
                    QString num=str1.at(1).mid(0,8);
                    currentData.codec=num;
                    QStringList str2=str1.at(1).split('\"');
                    if(str2.count()==2){
                        QString name=str2.at(1);
                        qDebug()<<name;
                        currentData.name=name;
                    }
                }
                currentData.TodayOpeningPrice=strList.at(1);
                currentData.YesterdayClosingPrice=strList.at(2);
                currentData.currentPrice=strList.at(3);
                currentData.todayMax=strList.at(4);
                currentData.todayMin=strList.at(5);
                currentData.Date=strList.at(30);
                currentData.Time=strList.at(31);
                if(!currentData.name.isEmpty() && !currentData.codec.isEmpty()){
                    ui->codecLabel->setText(currentData.name);
                    ui->currentPrice->setText(currentData.currentPrice);
                }
            }
        }

    }

}
void addMoneyWidget::on_cancelBtn_clicked()
{
    close();
}

void addMoneyWidget::on_setBtn_clicked()
{
    QMutexLocker locker(App->m_mutex);
    addGP();
    close();
}
