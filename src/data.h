#ifndef DATA_H
#define DATA_H
#include <QString>
#include <QMap>
struct DataGP{
    QString codec;
    QString name;
    QString TodayOpeningPrice;
    QString YesterdayClosingPrice;
    QString currentPrice;
    QString todayMax;
    QString todayMin;
    QString Date;
    QString Time;
    QString currentS;
};
struct DataHaveGP{
    QString codec;
    QString name;
    int haveNum=0;
    double payallPrice=0;
    double currentPrice=0;
    double currentallPrice=0;
    double historySY=0;
    double todaySY=0;
    double yesterDayPrice=0;
};

struct DataAllDP{
    double payallPrice=0;
    double currentallPrice=0;
    double todaySY=0;
};

struct MapdataGP
{
    QMap <QString ,DataGP> map;
};
struct MapdataHaveGP
{
    QMap <QString ,DataHaveGP> map;
};
class data
{
public:
    data();
};

#endif // DATA_H
