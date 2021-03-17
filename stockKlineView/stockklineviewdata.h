#ifndef STOCKKLINEVIEWDATA_H
#define STOCKKLINEVIEWDATA_H

#include <QObject>
#include <QTextStream>
#include <QVector>
#include <QObject>
class QTimer;
class QNetworkAccessManager;
class QNetworkReply;
class StockKlineViewData : public QObject
{
    Q_OBJECT
public:
    StockKlineViewData(QObject *parent=nullptr);
    void setData(const QString &code, const QString &dateStart,const QString &dateEnd);

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
};

#endif // STOCKKLINEVIEWDATA_H
