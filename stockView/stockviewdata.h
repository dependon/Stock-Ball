#ifndef STOCKVIEWDATA_H
#define STOCKVIEWDATA_H

#include <QFile>
#include <QString>
#include <stdlib.h>
#include <QTextStream>
#include <QVector>
#include <QObject>
class QTimer;
class QNetworkAccessManager;
class QNetworkReply;
typedef struct
{
    int  Date;
    int  Time;
    QString SecID;
    QString SecName;
    long Deal;
    long Vol;
    long Amt;
}FSJL;

typedef struct
{
    double deal_Start;
    double deal_Max;
    double deal_Min;
    double vol_Max;
    double vol_Min;
    double deal_rate;
}INFO;


class stockViewData :public QObject
{
    Q_OBJECT
public:
    stockViewData(QObject *parent=nullptr);
    bool ReadFSJL();

    void GetFSJLINFO();
    void setData(const QString &code, const QString &date);

    QVector <FSJL> fsjl;
    INFO info;
signals:
    void refreashView();
private slots:
    void replyFinished(QNetworkReply *reply);
    void updateData();
private:
    QNetworkAccessManager *manager {nullptr};
    QNetworkReply *reply {nullptr};
    QString szSecID;
    QString szDate;
    QTimer *m_timer{nullptr};
};

#endif // STOCKVIEWDATA_H
