#ifndef SIGNALM_H
#define SIGNALM_H

#include <QObject>
#include "data.h"
class signalM :public QObject
{
    Q_OBJECT
public:
    static signalM * instance();

signals:
    void sendDataGPsChange(MapdataGP);
    void sendDataHaveGPsChange(MapdataHaveGP);
    void sendDataAllDPChange(DataAllDP);
    void sendposthttpGp(const QString &);
    void sendaddMyGP(QString codec, int inum, double chasePrice);
    void sendremoveMyGP(QString codec, int inum=0, double chasePrice=0);
    void sendaddGP(const QString &str);
    void sendremoveGP(const QString &str);
    void sendExecDb(const QString &str);
    void refreashData();
    void refreashHaveData();

private:
    explicit signalM(QObject *parent = nullptr);
    static signalM* m_signal;
};

#endif // SIGNALM_H
