#ifndef STOCKKLINEVIEWDATA_H
#define STOCKKLINEVIEWDATA_H

#include <QWidget>
#include <QTextStream>
#include <QVector>
#include <QObject>
#include "datafile.h"
class QTimer;
class QNetworkAccessManager;
class QNetworkReply;
class kVolumeGrid;
class KLineGrid;

enum KlineEnum{
    DAYKLINE,
    WEEKKLINE,
    MONTHKLINE
};

class StockKlineViewData : public QWidget
{
    Q_OBJECT
public:
    StockKlineViewData(QWidget *parent=nullptr);
    void setData(const QString &code, const QString &dateStart,const QString &dateEnd);

    void setData(const QString &code, KlineEnum enumK);
    void updateData();
private slots:
    void replyFinished(QNetworkReply *reply);
private :
    QNetworkAccessManager *manager {nullptr};
    QNetworkReply *reply {nullptr};
    QString szSecID;
    QString szDateStart;
    QString szDateEnd;
    QTimer *m_timer{nullptr};
    kVolumeGrid *m_pgrid{nullptr};
    KLineGrid *m_klineGrid{nullptr};
    std::vector<KLine> m_vec;

    QString szSecCodec;
    KlineEnum m_enum{DAYKLINE};
};

#endif // STOCKKLINEVIEWDATA_H
