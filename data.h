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
};
class data
{
public:
    data();
};

#endif // DATA_H
