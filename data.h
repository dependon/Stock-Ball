#ifndef DATA_H
#define DATA_H
#include <QString>
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
    int haveNum;
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

class data
{
public:
    data();
};

#endif // DATA_H
