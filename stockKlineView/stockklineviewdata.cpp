#include "stockklineviewdata.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QEventLoop>
#include <QTimer>
StockKlineViewData::StockKlineViewData(QObject *parent)
    : QObject(parent)
{
    //创建一个管理器
    manager = new QNetworkAccessManager(this);
    //   reply = manager->get(request);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

void StockKlineViewData::setData(const QString &code, const QString &dateStart, const QString &dateEnd)
{
     szSecID=code;
     szDateStart=dateStart;
     szDateEnd=dateEnd;
     QTimer::singleShot(2000,[=]{
              updateData();
     });


}
void StockKlineViewData::updateData()
{
   QString str="q.stock.sohu.com/hisHq?code=cn_601766,cn_000002&start=20150601&end=20160414";
   QUrl url ;
//   url.setHost("https:");
//   url.setHost("https");
//   url.setQuery("https");
//   url.setScheme("https");
//   url.setFragment("http");
   url.setPath(str);
// url =QUrl("https://q.stock.sohu.com/hisHq?code=cn_"+QString(szSecID)+"&start="+szDateStart+"&end="+szDateEnd);
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    reply=manager->get(request);
}

void StockKlineViewData::replyFinished(QNetworkReply *reply)
{
    int i=0;
    QByteArray responseText = reply->readAll();
    QJsonDocument json = QJsonDocument::fromJson(responseText);
    QString date=json.object().value("date").toString();
    QString symbol=json.object().value("symbol").toString();
    int lastVolume=json.object().value("lastVolume").toInt();
    QString name=json.object().value("name").toString();
    double yestclose=json.object().value("yestclose").toDouble();
    int count=json.object().value("count").toInt();
    QJsonArray data=json.object().value("data").toArray();

    reply->deleteLater();
//    emit refreashView();
}
