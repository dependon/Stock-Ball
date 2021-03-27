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
#ifdef Q_OS_LINUX
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


//        str = "http://cq.ssajax.cn/interact/getTradedata.ashx?pic=qlpic_000001_1_6";

        QString string;
        string.prepend(byte);// QByteArray转QString方法2
        QUrl url = QUrl::fromEncoded(byte);
        QNetworkRequest request;
        request.setUrl(QUrl(str));
        reply = manager->get(request);
    }
#else
#endif


}

void StockKlineViewData::replyFinished(QNetworkReply *reply)
{
#ifdef Q_OS_LINUX
    m_vec.clear();
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
#else
#endif

}
