#include "stockviewdata.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QEventLoop>
#include <QTimer>
stockViewData::stockViewData(QObject *parent)
    :QObject (parent)
{
    //创建一个管理器
    manager = new QNetworkAccessManager(this);
    //   reply = manager->get(request);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    szSecID = "0000001" ;
    szDate  ;
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &stockViewData::updateData);
    m_timer->start(1000);
}
void stockViewData::setData(const QString &code, const QString &date)
{
    szSecID = code ;
    szDate  =date;
    updateData();
}
void stockViewData::GetFSJLINFO()
{

    if(fsjl.count()<1){
        return;
    }
    double UpRate = 0, DnRate = 0;
    info.deal_Start = info.deal_Max = info.deal_Min = fsjl[0].Deal;
    info.vol_Max = info.vol_Min = 0;

    for( auto fs: fsjl )
    {
        if( info.deal_Max < fs.Deal )
            info.deal_Max = fs.Deal;
        if( info.deal_Min > fs.Deal )
            info.deal_Min = fs.Deal;

        if(info.vol_Max < fs.Vol)
            info.vol_Max = fs.Vol;
        if(info.vol_Min > fs.Vol)
            info.vol_Min = fs.Vol;
    }

    qDebug("%d",info.deal_Max);

    if( info.deal_Max > info.deal_Start)
    {
        UpRate = ( info.deal_Max - info.deal_Start ) / info.deal_Start ;
    }
    if( info.deal_Min < info.deal_Start)
    {
        DnRate = ( info.deal_Start - info.deal_Min ) / info.deal_Start ;
    }

    info.deal_rate = UpRate > DnRate ? UpRate : DnRate ;

}

void stockViewData::replyFinished(QNetworkReply *reply)
{
    fsjl.clear();
    int i=0;
    QByteArray responseText = reply->readAll();
    QJsonDocument json = QJsonDocument::fromJson(responseText);
    QString date=json.object().value("date").toString();
    QString symbol=json.object().value("symbol").toString();
    int lastVolume=json.object().value("lastVolume").toInt();
    QString name=json.object().value("name").toString();
    int yestclose=json.object().value("yestclose").toInt();
    int count=json.object().value("count").toInt();
    QJsonArray data=json.object().value("data").toArray();
    for(QJsonValue value:data){
        FSJL jl;

         QString time=value.toArray()[0].toString();
         double nubmer1=value.toArray()[1].toDouble();
         double nubmer2=value.toArray()[2].toDouble();
         int vol=value.toArray()[3].toInt();

         jl.Date=date.toInt();
         QString timeN=time+"00";
         jl.Time=timeN.toInt();
         jl.SecID=symbol;
         jl.SecName=name;
         jl.Deal=nubmer1*1000;
         jl.Vol=vol;
         fsjl.push_back(jl);
         i++;
    }
    GetFSJLINFO();
    reply->deleteLater();
    emit refreashView();
}

void stockViewData::updateData()
{
    QUrl url =QString("http://img1.money.126.net/data/hs/time/today/"+QString(szSecID)+".json");
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    reply=manager->get(request);
}



bool stockViewData::ReadFSJL()
{
    return true;
}
