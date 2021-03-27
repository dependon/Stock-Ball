#include "stockklineviewdata.h"
#include "kvolumegrid.h"
#include "klinegrid.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QEventLoop>
#include <QTimer>
#include <QTextCodec>
#include <QUrl>
#include <QVBoxLayout>

StockKlineViewData::StockKlineViewData(QWidget *parent)
    : QWidget(parent)
{
    //    QTextCodec *codec = QTextCodec::codecForName("UTF-8");//或者"GBK",不分大小写
    //    QTextCodec::setCodecForLocale(codec);
    //创建一个管理器
    manager = new QNetworkAccessManager(this);
    //   reply = manager->get(request);
    connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));
    //    m_pgrid = new kVolumeGrid(nullptr);
    //    m_pgrid->setObjectName(tr("kline"));
    //    m_pgrid->setFocusPolicy(Qt::StrongFocus);
    //    m_pgrid->show();

    QVBoxLayout *layout = new QVBoxLayout(this);
    this->setLayout(layout);

    m_klineGrid = new KLineGrid();
    this->layout()->addWidget(m_klineGrid);
    //    m_klineGrid->setMinimumSize(QSize(400,300));

}

void StockKlineViewData::setData(const QString &code, const QString &dateStart, const QString &dateEnd)
{
    szSecID = code;
    szDateStart = dateStart;
    szDateEnd = dateEnd;
    updateData();


}

void StockKlineViewData::setData(const QString &code, KlineEnum enumK)
{
    szSecCodec = code;
    m_enum = enumK;
    updateData();
}
void StockKlineViewData::updateData()
{
    if (!szSecID.isEmpty()) {
        QString str = "https://q.stock.sohu.com/hisHq?code=cn_" + szSecID + "&start=" + szDateStart + "&end=" + szDateEnd;
        QByteArray byte = str.toLatin1();
        QString string;
        string.prepend(byte);// QByteArray转QString方法2
        QUrl url = QUrl::fromEncoded(byte);
        QNetworkRequest request;
        request.setUrl(url);
        reply = manager->get(request);
    } else if (!szSecCodec.isEmpty()) {
        //    https://data.gtimg.cn/flashdata/hushen/latest/daily/sz000002.js?maxage=43201&visitDstTime=1
        QString str;
        if (m_enum == DAYKLINE) {
            str = "https://data.gtimg.cn/flashdata/hushen/latest/daily/" + szSecCodec + ".js?maxage=43201&visitDstTime=1";
        } else if (m_enum == WEEKKLINE) {
            //http://data.gtimg.cn/flashdata/hushen/latest/weekly/sz000002.js?maxage=43201&visitDstTime=1
            str = "https://data.gtimg.cn/flashdata/hushen/latest/weekly/" + szSecCodec + ".js?maxage=43201&visitDstTime=1";
        } else {
            str = "https://data.gtimg.cn/flashdata/hushen/monthly/" + szSecCodec + ".js?maxage=43201";
        }
        QByteArray byte = str.toLatin1();
        QString string;
        string.prepend(byte);// QByteArray转QString方法2
        QUrl url = QUrl::fromEncoded(byte);
        QNetworkRequest request;
        request.setUrl(url);
        reply = manager->get(request);
    }



}

void StockKlineViewData::replyFinished(QNetworkReply *reply)
{
//#ifdef 0
//    m_vec.clear();


//    int i=-1;
//    QByteArray responseText = reply->readAll();
//    QJsonDocument json = QJsonDocument::fromJson(responseText);
//    i=json.object().value("status").toInt();

//    //    std::vector<KLine> vec;
//    for(QJsonValue value:json.array()){
//        int status=value.toObject().value("status").toInt();
//        QString code= value.toObject().value("code").toString();
//        QJsonArray hq=value.toObject().value("hq").toArray();
//        for(int index=hq.count()-1;index>=0;index--){
//            QJsonValue value1=hq.takeAt(index);
//            KLine tmp;
//            QString time=value1.toArray()[0].toString();//日期
//            QString kp=value1.toArray()[1].toString();//开盘
//            QString sp=value1.toArray()[2].toString();//收盘
//            QString zd=value1.toArray()[3].toString();//涨跌
//            QString zf=value1.toArray()[4].toString();//涨幅
//            QString min=value1.toArray()[5].toString();//最低
//            QString max=value1.toArray()[6].toString();//最高
//            QString cjl=value1.toArray()[7].toString();//成交量
//            QString cje=value1.toArray()[8].toString();//成交额
//            QString hs=value1.toArray()[9].toString();//换手
//            qDebug()<<"ok";
//            tmp.time=time;
//            tmp.openingPrice=kp.toDouble();
//            tmp.closeingPrice=sp.toDouble();
//            tmp.highestBid=max.toDouble();
//            tmp.lowestBid=min.toDouble();
//            tmp.amountOfIncrease=zf.toDouble();
//            tmp.amountOfAmplitude;
//            tmp.totalVolume=cjl;
//            tmp.totalAmount=cje;
//            tmp.turnoverRate=hs.toDouble();
//            m_vec.push_back(tmp);
//        }

//    }
//    //    m_pgrid->readData(vec);
//    m_klineGrid->readData(m_vec);

//    reply->deleteLater();
//    //    emit refreashView();
//#endif

    m_vec.clear();


    int i = -1;
//    QByteArray responseText = reply->readAll();
//    QList<QByteArray> rawHeaderList = reply->rawHeaderList();
//    qDebug()<<responseText;
//    QJsonDocument json = QJsonDocument::fromJson(responseText);
//    i=json.object().value("status").toInt();
//    qDebug()<<reply->size();
    while (reply->canReadLine()) {
        QByteArray line = reply->readLine(0);
//        qDebug()<<line;
        QString str(line);
        QStringList strlist = str.split(" ");
        if (strlist.size() == 6) {
            strlist[5] = strlist[5].replace("\\n\\\n", "");


            KLine tmp;
            QString time = strlist[0]; //日期
            QString kp = strlist[1]; //开盘
            QString sp = strlist[2]; //收盘
            QString zd = QString::number((strlist[2].toDouble() - strlist[1].toDouble())); //涨跌
            double a = (strlist[2].toDouble() - strlist[1].toDouble()) * 100 / strlist[1].toDouble();
            QString zf = QString::number(a) + "%"; //涨幅
            QString min = strlist[4]; //最低
            QString max = strlist[3]; //最高
            QString cjl = strlist[5]; //成交量
            //            QString cje=value1.toArray()[8].toString();//成交额
            //            QString hs=value1.toArray()[9].toString();//换手
            //            qDebug()<<"ok";
            tmp.time = time;
            tmp.openingPrice = kp.toDouble();
            tmp.closeingPrice = sp.toDouble();
            tmp.highestBid = max.toDouble();
            tmp.lowestBid = min.toDouble();
            tmp.amountOfIncrease = zf.toDouble();
            tmp.amountOfAmplitude;
            tmp.totalVolume = cjl;
//                        tmp.totalAmount=cje;
//                        tmp.turnoverRate=hs.toDouble();
            m_vec.push_back(tmp);
        }
    }
    m_klineGrid->readData(m_vec);
    reply->deleteLater();
}
