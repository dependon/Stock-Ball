#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QSystemTrayIcon>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTextCodec>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QMutex>
#include <QMutexLocker>
#include <QColor>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QFileInfo>

#include <application.h>
#include <addmoneywidget.h>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_trayIcon=new QSystemTrayIcon();
    m_trayIcon->setToolTip("test");
    m_trayIcon->show();
    initDB();
    //创建一个管理器
    manager = new QNetworkAccessManager(this);
    manager2=new QNetworkAccessManager(this);
    //   reply = manager->get(request);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    connect(manager2, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished2(QNetworkReply*)));
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateData()));
    m_timer->start(1000);

    ui->tableWidget->hide();


}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::initDB()
{
    App->m_db=QSqlDatabase::addDatabase("QSQLITE");
    QFile file("test.db");
    if(file.exists())
    {
        qDebug()<<"文件存在";
    }else{
        qDebug()<<"文件不存在,正在新建文件.";
        file.open( QIODevice::ReadWrite | QIODevice::Text );
        file.close();
    }
    App->m_db.setDatabaseName("test.db");

    //打开数据库
    if (! App->m_db.isValid()) {
        QMessageBox::warning(this, "错误", App->m_db.lastError().text());
        return;
    }
    if(!App->m_db.open())
    {
        QMessageBox::warning(this, "错误", App->m_db.lastError().text());
        return;
    }
    QSqlQuery querycreate;
    querycreate.exec("CREATE TABLE IF NOT EXISTS myData(code Text primary key)");
    querycreate.exec("CREATE TABLE IF NOT EXISTS haveData(code Text primary key,money Text,number INTEGER)");
    QSqlQuery query;
    query.exec("select * from myData");

    while(query.next()) //一行一行遍历
    {
        //取出当前行的内容
        //以列为单位的     //第0列
        QString str= query.value(0).toString();
        m_mGp.insert(str,DataGP());
    }

    QSqlQuery query2;
    query2.exec("select * from haveData");
    while(query2.next()) //一行一行遍历
    {
        DataHaveGP gp;
        //取出当前行的内容
        //以列为单位的     //第0列
        QString str= query2.value(0).toString();
        QString str2= query2.value(1).toString();
        int num= query2.value(2).toInt();
        gp.codec=str;
        gp.payallPrice=str2.toDouble();
        gp.haveNum=num;
        m_mMyGp.insert(str,gp);
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString bumStr="http://hq.sinajs.cn/list="+m_currentZQ+ui->searchEdit->text();
    QNetworkRequest request;
    request.setUrl(QUrl(bumStr));
    reply=manager->get(request);

}

QByteArray MainWindow::getReply(QString surl)
{
    QNetworkAccessManager *NAM = new QNetworkAccessManager;
    QNetworkRequest request;
    request.setUrl(QUrl(surl));
    QNetworkReply *reply = NAM->get(request);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    reply->deleteLater();
    return reply->readAll();
}

void MainWindow::sendData()
{
    for(QString key:m_mGp.keys()){
        QString bumStr="http://hq.sinajs.cn/list="+key;
        QNetworkRequest request;
        request.setUrl(QUrl(bumStr));
        reply=manager->get(request);
    }
}
void MainWindow::sendData2()
{
    for(QString key:m_mMyGp.keys()){
        QString bumStr="http://hq.sinajs.cn/list="+key;
        QNetworkRequest request;
        request.setUrl(QUrl(bumStr));
        reply2=manager2->get(request);
    }
}
void MainWindow::addGP(const QString &str)
{
    if(App->m_db.isValid()){
        QSqlQuery query(App->m_db);
        //占位符 : + 自定义名字
        query.prepare("insert into myData(code) values(:code)");
        query.bindValue(":code", str);
        query.setForwardOnly(true);
        query.exec();
    }
}

void MainWindow::removeGP(const QString &str)
{

    if(App->m_db.isValid()){
        QSqlQuery query(App->m_db);
        QString queryStr=QString("delete from myData where code=\"%1\" ").arg(str);

        //        query.prepare("delete from myData where code=:code");
        //        query.bindValue(":code", str);
        query.setForwardOnly(true);
        //        query.exec();
        query.exec(queryStr);

//        QString qs = "delete from myData where code=\"?\"";
//        query.prepare(qs);
//        query.addBindValue(str);
//        query.execBatch();
//        query.exec("COMMIT");
        m_mGp.remove(str);
        m_mGp;
    }
}


//数据依次是“股票名称、今日开盘价、昨日收盘价、当前价格、今日最高价、
//今日最低价、竞买价、竞卖价、成交股数、成交金额、买1手、买1报价、买2手、买2报价、
//…、买5报价、…、卖5报价、日期、时间”。
void MainWindow::replyFinished(QNetworkReply* reply)
{
    QMutexLocker locker(App->m_mutex);
    QByteArray data = reply->readAll();
    QTextCodec *tc = QTextCodec::codecForName("GBK");

    QString str = tc->toUnicode(data);//str如果是中文则是中文字符
    str.remove("\n");

    QList<QString> numList=str.split(";");
    for(QString numStr:numList)
    {
        if(!numStr.isEmpty()){
            QList<QString> strList=numStr.split(",");
            DataGP gp;
            if(strList.count()>31){
                QStringList str1=strList.at(0).split("str_");
                if(str1.count()==2){
                    QString num=str1.at(1).mid(0,8);
                    gp.codec=num;
                    QStringList str2=str1.at(1).split('\"');
                    if(str2.count()==2){
                        QString name=str2.at(1);
                        qDebug()<<name;
                        gp.name=name;
                    }
                }
                gp.TodayOpeningPrice=strList.at(1);
                gp.YesterdayClosingPrice=strList.at(2);
                gp.currentPrice=strList.at(3);
                gp.todayMax=strList.at(4);
                gp.todayMin=strList.at(5);
                gp.Date=strList.at(30);
                gp.Time=strList.at(31);
                if(!gp.name.isEmpty() && !gp.codec.isEmpty()){
                    if(!m_mGp.contains(gp.codec)){
                        addGP(gp.codec);
                    }
                    m_mGp.insert(gp.codec,gp);
                }
            }
        }

    }



    //        QJsonDocument JD = QJsonDocument::fromJson(str.toLatin1());
    //        int status = JD.object().value("").toInt();
    qDebug()<<"xxx";
}

void MainWindow::replyFinished2(QNetworkReply *reply)
{
    QMutexLocker locker(App->m_mutex);
    QByteArray data = reply->readAll();
    QTextCodec *tc = QTextCodec::codecForName("GBK");

    QString str = tc->toUnicode(data);//str如果是中文则是中文字符
    str.remove("\n");

    QList<QString> numList=str.split(";");
    for(QString numStr:numList)
    {
        if(!numStr.isEmpty()){
            QList<QString> strList=numStr.split(",");
            DataGP gp;
            if(strList.count()>31){
                QStringList str1=strList.at(0).split("str_");
                if(str1.count()==2){
                    QString num=str1.at(1).mid(0,8);
                    gp.codec=num;
                    QStringList str2=str1.at(1).split('\"');
                    if(str2.count()==2){
                        QString name=str2.at(1);
                        qDebug()<<name;
                        gp.name=name;
                    }
                }
                gp.TodayOpeningPrice=strList.at(1);
                gp.YesterdayClosingPrice=strList.at(2);
                gp.currentPrice=strList.at(3);
                gp.todayMax=strList.at(4);
                gp.todayMin=strList.at(5);
                gp.Date=strList.at(30);
                gp.Time=strList.at(31);
                if(!gp.name.isEmpty() && !gp.codec.isEmpty()){

                  DataHaveGP haveGp=  m_mMyGp.value(gp.codec);
                  haveGp.name=gp.name;
                  haveGp.currentPrice=gp.currentPrice.toDouble();
                  haveGp.currentallPrice=gp.currentPrice.toDouble()*haveGp.haveNum;
                  haveGp.todaySY=(gp.currentPrice.toDouble()-gp.YesterdayClosingPrice.toDouble())*haveGp.haveNum;
                  haveGp.historySY=(haveGp.currentallPrice-haveGp.payallPrice);
                  haveGp.yesterDayPrice=gp.YesterdayClosingPrice.toDouble();
                  m_mMyGp.insert(gp.codec,haveGp);
                }
            }
        }

    }
}
void MainWindow::updateData()
{
    QMutexLocker locker(App->m_mutex);
    if(m_mGp.count()>0){
        sendData();
        sendData2();
        ui->tableWidget->setRowCount(m_mGp.count());
        ui->miniTable->setRowCount(m_mGp.count());
        int index=0;
        for(auto gp:m_mGp){
            ui->tableWidget->setItem(index,0,new QTableWidgetItem(gp.name));
            ui->tableWidget->setItem(index,1,new QTableWidgetItem(gp.codec));
            ui->tableWidget->setItem(index,2,new QTableWidgetItem(gp.TodayOpeningPrice));
            ui->tableWidget->setItem(index,3,new QTableWidgetItem(gp.YesterdayClosingPrice));
            ui->tableWidget->setItem(index,4,new QTableWidgetItem(gp.currentPrice));
            ui->tableWidget->setItem(index,5,new QTableWidgetItem(gp.todayMax));
            ui->tableWidget->setItem(index,6,new QTableWidgetItem(gp.todayMin));
            ui->tableWidget->setItem(index,7,new QTableWidgetItem(gp.Date));
            ui->tableWidget->setItem(index,8,new QTableWidgetItem(gp.Time));
            QString name1=gp.name;
            QString codec=gp.codec;
            QString cuurent=gp.currentPrice;
            double zd=gp.currentPrice.toDouble()-gp.YesterdayClosingPrice.toDouble();
            QString zdStr=QString::number(zd);
            double zf=zd*100.0/gp.YesterdayClosingPrice.toDouble();
            QString zfStr=QString::number(zf)+"%";
            QTableWidgetItem *nameitem = new QTableWidgetItem(name1);
            nameitem->setData(1,gp.codec);
            QTableWidgetItem *currentItem = new QTableWidgetItem(cuurent);
            QTableWidgetItem *zdStrItem = new QTableWidgetItem(zdStr);
            QTableWidgetItem *zfStrItem = new QTableWidgetItem(zfStr);
            if(zf>0){
                nameitem->setTextColor(QColor("white"));
                nameitem->setBackground(QColor("red"));
                currentItem->setTextColor(QColor("white"));
                currentItem->setBackground(QColor("red"));
                zdStrItem->setTextColor(QColor("white"));
                zdStrItem->setBackground(QColor("red"));
                zfStrItem->setTextColor(QColor("white"));
                zfStrItem->setBackground(QColor("red"));
            }
            else{
                nameitem->setTextColor(QColor("white"));
                nameitem->setBackground(QColor("green"));
                currentItem->setTextColor(QColor("white"));
                currentItem->setBackground(QColor("green"));
                zdStrItem->setTextColor(QColor("white"));
                zdStrItem->setBackground(QColor("green"));
                zfStrItem->setTextColor(QColor("white"));
                zfStrItem->setBackground(QColor("green"));
            }
            ui->miniTable->setItem(index,0,nameitem);
            ui->miniTable->setItem(index,1,currentItem);
            ui->miniTable->setItem(index,2,zdStrItem);
            ui->miniTable->setItem(index,3,zfStrItem);

            index++;
        }



    }
    m_myAllDP=DataAllDP();
    ui->myTable->setRowCount(m_mMyGp.count()+1);
    int index1=0;
    DataHaveGP allgp;
    for(auto gp:m_mMyGp){
        allgp.haveNum+=gp.haveNum;
        allgp.todaySY+=gp.todaySY;
        allgp.historySY+=gp.historySY;
        allgp.payallPrice+=gp.payallPrice;
        allgp.currentPrice+=gp.currentPrice;
        allgp.yesterDayPrice+=gp.yesterDayPrice;
        allgp.currentallPrice+=gp.currentallPrice;
        ui->myTable->setItem(index1,0,new QTableWidgetItem(gp.name));

        ui->myTable->setItem(index1,1,new QTableWidgetItem(QString::number(gp.haveNum)));
        ui->myTable->setItem(index1,2,new QTableWidgetItem(QString::number(gp.payallPrice)));
        ui->myTable->setItem(index1,3,new QTableWidgetItem(QString::number(gp.currentallPrice)));
        ui->myTable->setItem(index1,4,new QTableWidgetItem(QString::number(gp.currentPrice)));
        ui->myTable->setItem(index1,5,new QTableWidgetItem(QString::number(gp.historySY)));
        double historySyl=100*gp.historySY/gp.payallPrice;


        QString historyL=QString::number(historySyl)+"%";
        ui->myTable->setItem(index1,6,new QTableWidgetItem(historyL));
        if(historySyl>0){
            ui->myTable->item(index1,5)->setTextColor(QColor("white"));
            ui->myTable->item(index1,6)->setTextColor(QColor("white"));
            ui->myTable->item(index1,5)->setBackground(QColor("red"));
            ui->myTable->item(index1,6)->setBackground(QColor("red"));
        }
        else {
            ui->myTable->item(index1,5)->setTextColor(QColor("white"));
            ui->myTable->item(index1,6)->setTextColor(QColor("white"));
            ui->myTable->item(index1,5)->setBackground(QColor("green"));
            ui->myTable->item(index1,6)->setBackground(QColor("green"));
        }
        ui->myTable->setItem(index1,7,new QTableWidgetItem(QString::number(gp.todaySY)));
        double todayyl=100*(gp.currentPrice-gp.yesterDayPrice)/gp.yesterDayPrice;
        QString todayL=QString::number(todayyl)+"%";
        ui->myTable->setItem(index1,8,new QTableWidgetItem(todayL));
        if(todayyl>0){
            ui->myTable->item(index1,7)->setTextColor(QColor("white"));
            ui->myTable->item(index1,8)->setTextColor(QColor("white"));
            ui->myTable->item(index1,7)->setBackground(QColor("red"));
            ui->myTable->item(index1,8)->setBackground(QColor("red"));
        }
        else {
            ui->myTable->item(index1,7)->setTextColor(QColor("white"));
            ui->myTable->item(index1,8)->setTextColor(QColor("white"));
            ui->myTable->item(index1,7)->setBackground(QColor("green"));
            ui->myTable->item(index1,8)->setBackground(QColor("green"));
        }
        m_myAllDP.payallPrice+=gp.payallPrice;
        m_myAllDP.currentallPrice+=gp.currentallPrice;
        m_myAllDP.todaySY+=gp.todaySY;
        index1++;
    }
    ui->myTable->setItem(index1,0,new QTableWidgetItem(allgp.name));

    ui->myTable->setItem(index1,1,new QTableWidgetItem(QString::number(allgp.haveNum)));
    ui->myTable->setItem(index1,2,new QTableWidgetItem(QString::number(allgp.payallPrice)));
    ui->myTable->setItem(index1,3,new QTableWidgetItem(QString::number(allgp.currentallPrice)));
    ui->myTable->setItem(index1,4,new QTableWidgetItem(QString::number(allgp.currentPrice)));
    ui->myTable->setItem(index1,5,new QTableWidgetItem(QString::number(allgp.historySY)));
    double historySyl=100*allgp.historySY/allgp.payallPrice;


    QString historyL=QString::number(historySyl)+"%";
    ui->myTable->setItem(index1,6,new QTableWidgetItem(historyL));
    if(historySyl>0){
        ui->myTable->item(index1,5)->setTextColor(QColor("white"));
        ui->myTable->item(index1,6)->setTextColor(QColor("white"));
        ui->myTable->item(index1,5)->setBackground(QColor("red"));
        ui->myTable->item(index1,6)->setBackground(QColor("red"));
    }
    else {
        ui->myTable->item(index1,5)->setTextColor(QColor("white"));
        ui->myTable->item(index1,6)->setTextColor(QColor("white"));
        ui->myTable->item(index1,5)->setBackground(QColor("green"));
        ui->myTable->item(index1,6)->setBackground(QColor("green"));
    }
    ui->myTable->setItem(index1,7,new QTableWidgetItem(QString::number(allgp.todaySY)));
    double todayyl=100*(allgp.currentPrice-allgp.yesterDayPrice)/allgp.yesterDayPrice;
    QString todayL=QString::number(todayyl)+"%";
    ui->myTable->setItem(index1,8,new QTableWidgetItem(todayL));
    if(todayyl>0){
        ui->myTable->item(index1,7)->setTextColor(QColor("white"));
        ui->myTable->item(index1,8)->setTextColor(QColor("white"));
        ui->myTable->item(index1,7)->setBackground(QColor("red"));
        ui->myTable->item(index1,8)->setBackground(QColor("red"));
    }
    else {
        ui->myTable->item(index1,7)->setTextColor(QColor("white"));
        ui->myTable->item(index1,8)->setTextColor(QColor("white"));
        ui->myTable->item(index1,7)->setBackground(QColor("green"));
        ui->myTable->item(index1,8)->setBackground(QColor("green"));
    }
    cureentInfo="今日总收益:" +QString::number(allgp.todaySY) +" \n" +" 今日收益率"+ todayL;
    m_trayIcon->setToolTip(cureentInfo);
}

void MainWindow::on_comboBox_activated(const QString &arg1)
{
    if(arg1.contains("上")){
        m_currentZQ="sh";
    }
    else if(arg1.contains("深")){
        m_currentZQ="sz";
    }
}

void MainWindow::on_miniTable_doubleClicked(const QModelIndex &index)
{
    QMutexLocker locker(App->m_mutex);
    if(ui->miniTable->currentRow()>=0){
        QString code=ui->miniTable->item(ui->miniTable->currentRow(),0)->data(1).toString();
        if(!code.isEmpty()){
            removeGP(code);
        }
    }
}

void MainWindow::on_miniTable_itemDoubleClicked(QTableWidgetItem *item)
{
    if(item){

    }
}

void MainWindow::on_normalBtn_clicked()
{
    if(ui->miniTable->isVisible()){
        ui->miniTable->setVisible(false);
    }
    else {
        ui->miniTable->setVisible(true);
    }
}

void MainWindow::on_detailedBtn_clicked()
{
    if(ui->tableWidget->isVisible()){
        ui->tableWidget->setVisible(false);
    }
    else {
        ui->tableWidget->setVisible(true);
    }
}

void MainWindow::on_myBtn_clicked()
{
    if(ui->myTable->isVisible()){
        ui->myTable->setVisible(false);
    }
    else {
        ui->myTable->setVisible(true);
    }
}

void MainWindow::on_addMyBtn_clicked()
{
    addMoneyWidget addDialog;
    addDialog.setCodecData(m_mGp);
    addDialog.exec();
}
