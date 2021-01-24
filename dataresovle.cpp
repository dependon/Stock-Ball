#include "dataresovle.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTextCodec>
#include <QTimer>
#include <QString>
#include <QWidget>

#include "signalm.h"
DataResovle *DataResovle::m_dataResovle =nullptr;
DataResovle *DataResovle::instance()
{
    if (! m_dataResovle) {
        m_dataResovle = new DataResovle();
    }

    return m_dataResovle;
}

void DataResovle::setSql(const QString &str)
{
    if(m_db.isValid()){
        QSqlQuery query(m_db);
        query.exec(str);
    }
}
void DataResovle::addGP(const QString &str)
{
    if(m_db.isValid()){
        QSqlQuery query(m_db);
        //占位符 : + 自定义名字
        query.prepare("insert into myData(code) values(:code)");
        query.bindValue(":code", str);
        query.setForwardOnly(true);
        query.exec();
    }
}
DataResovle::DataResovle(QObject *parent) : QObject(parent)
{
    m_mutex=new QMutex();
    initDB();
    initConect();

}

void DataResovle::initDB()
{
    m_db=QSqlDatabase::addDatabase("QSQLITE");
    QFile file("test.db");
    if(file.exists())
    {
        qDebug()<<"文件存在";
    }else{
        qDebug()<<"文件不存在,正在新建文件.";
        file.open( QIODevice::ReadWrite | QIODevice::Text );
        file.close();
    }
    m_db.setDatabaseName("test.db");

    //打开数据库
    if (! m_db.isValid()) {
        qDebug()<<"error";
        return;
    }
    if(!m_db.open())
    {
        qDebug()<<"error";
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
void DataResovle::removeGP(const QString &str)
{
    if(m_db.isValid()){
        QSqlQuery query(m_db);
        QString queryStr=QString("delete from myData where code=\"%1\" ").arg(str);
        query.setForwardOnly(true);
        query.exec(queryStr);
        m_mGp.remove(str);
    }
    if(0==m_mGp.count()){
        MapdataGP mapGp;
        mapGp.map=m_mGp;
        emit signalM::instance()->sendDataGPsChange(mapGp);
    }
}
void DataResovle::initConect()
{
    //创建一个管理器
    manager = new QNetworkAccessManager(this);
    manager2=new QNetworkAccessManager(this);
    //   reply = manager->get(request);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    connect(manager2, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished2(QNetworkReply*)));

    //    void addMyGP(QString codec, int inum, double chasePrice);
    //    void removeMyGP(QString codec, int inum=0, double chasePrice=0);
    //    void addGP(const QString &str);
    //    void removeGP(const QString &str);

    connect(signalM::instance(),&signalM::sendaddMyGP,this,&DataResovle::addMyGP);
    connect(signalM::instance(),&signalM::sendremoveMyGP,this,&DataResovle::removeMyGP);
    connect(signalM::instance(),&signalM::sendaddGP,this,&DataResovle::addGP);
    connect(signalM::instance(),&signalM::sendremoveGP,this,&DataResovle::removeGP);

    connect(signalM::instance(),&signalM::sendposthttpGp,this,&DataResovle::sendHttpData);

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateData()));
    m_timer->start(1000);
}
//数据依次是“股票名称、今日开盘价、昨日收盘价、当前价格、今日最高价、
//今日最低价、竞买价、竞卖价、成交股数、成交金额、买1手、买1报价、买2手、买2报价、
//…、买5报价、…、卖5报价、日期、时间”。
void DataResovle::replyFinished(QNetworkReply* reply)
{
    QMutexLocker locker(m_mutex);
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
    MapdataGP mapGp;
    mapGp.map=m_mGp;
    emit signalM::instance()->sendDataGPsChange(mapGp);
}

void DataResovle::replyFinished2(QNetworkReply *reply)
{
    QMutexLocker locker(m_mutex);
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
                    haveGp.codec=gp.codec;
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
    MapdataHaveGP mapGp;
    mapGp.map=m_mMyGp;
    emit signalM::instance()->sendDataHaveGPsChange(mapGp);
}
void DataResovle::sendHttpData(const QString& str)
{
    QString bumStr="http://hq.sinajs.cn/list="+str;
    QNetworkRequest request;
    request.setUrl(QUrl(bumStr));
    reply=manager->get(request);
}
void DataResovle::sendData()
{
    for(QString key:m_mGp.keys()){
        QString bumStr="http://hq.sinajs.cn/list="+key;
        QNetworkRequest request;
        request.setUrl(QUrl(bumStr));
        reply=manager->get(request);
    }
}
void DataResovle::sendData2()
{
    for(QString key:m_mMyGp.keys()){
        QString bumStr="http://hq.sinajs.cn/list="+key;
        QNetworkRequest request;
        request.setUrl(QUrl(bumStr));
        reply2=manager2->get(request);
    }
}


void DataResovle::updateData()
{
    QMutexLocker locker(m_mutex);
    if(m_mGp.count()>0){
        sendData();
    }
    if(m_mMyGp.count()>0){
        sendData2();
    }

}
void DataResovle::addMyGP(QString codec,int inum,double chasePrice)
{

    if(m_db.isValid()){

        double dnum=inum;
        double addMoney =dnum*chasePrice;
        QSqlQuery query(m_db);
        //占位符 : + 自定义名字
        //        query.prepare("insert into myData(code) values(:code)");
        QString insert_sql = "insert into haveData values (?, ?, ?)";
        query.prepare(insert_sql);
        query.addBindValue(codec);
        query.addBindValue(QString::number(addMoney));
        query.addBindValue(inum);
        //        query.bindValue(":code", str);
        query.setForwardOnly(true);
        query.exec();
        DataHaveGP gp;
        gp.codec=codec;
        gp.payallPrice=chasePrice;
        gp.haveNum=inum;
        m_mMyGp.insert(codec,gp);
    }
}

void DataResovle::removeMyGP(QString codec, int inum, double chasePrice)
{
    if(m_db.isValid()){
        QSqlQuery query(m_db);
        QString queryStr=QString("delete from haveData where code=\"%1\" ").arg(codec);
        query.setForwardOnly(true);
        query.exec(queryStr);
        m_mMyGp.remove(codec);
    }
    if(0==m_mMyGp.count()){
        MapdataHaveGP mapGp;
        mapGp.map=m_mMyGp;
        emit signalM::instance()->sendDataHaveGPsChange(mapGp);
    }
}
