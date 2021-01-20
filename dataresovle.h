#ifndef DATARESOVLE_H
#define DATARESOVLE_H
#include <QObject>
#include <QMutex>
#include <QSqlDatabase>
#include <QMap>
#include "data.h"
class QTimer;
class QNetworkAccessManager;
class QNetworkReply;

class DataResovle  : public QObject
{
    Q_OBJECT
public:
    static DataResovle * instance();

    void setSql(const QString & str);

private slots:
    void replyFinished(QNetworkReply *reply);
    void replyFinished2(QNetworkReply *reply);
    void sendHttpData(const QString &str);
    void sendData();
    void sendData2();
    void updateData();
    void addMyGP(QString codec, int inum, double chasePrice);
    void removeMyGP(QString codec, int inum=0, double chasePrice=0);
    void addGP(const QString &str);
    void removeGP(const QString &str);
private:
    void initDB();
    void initConect();
    explicit DataResovle(QObject *parent = nullptr);

    QSqlDatabase m_db;
    QMutex* m_mutex{nullptr};
    static DataResovle *m_dataResovle;

    QNetworkAccessManager *manager {nullptr};
    QNetworkAccessManager *manager2 {nullptr};
    QNetworkReply *reply {nullptr};
    QNetworkReply *reply2 {nullptr};
    QMap <QString,DataGP> m_mGp;
    QMap <QString,DataHaveGP> m_mMyGp;
    DataAllDP m_myAllDP;
    QTimer *m_timer{nullptr};


};

#endif // DATARESOVLE_H
