#include "mainwindow.h"
#include <application.h>
#include "data.h"
#include <QMetaType>
#include "floatball.h"
#include "stackstock.h"
#include "stockKlineView/stockklineviewdata.h"
#include <QDateTime>
Q_DECLARE_METATYPE(MapdataHaveGP);
Q_DECLARE_METATYPE(MapdataGP);
Q_DECLARE_METATYPE(DataAllDP);
int main(int argc, char *argv[])
{

    qRegisterMetaType< MapdataGP>("MapdataGP");
    qRegisterMetaType< MapdataHaveGP>("MapdataHaveGP");
    qRegisterMetaType<DataHaveGP>("DataHaveGP");
    qRegisterMetaType<DataGP>("DataGP");
    qRegisterMetaType<DataAllDP>("DataAllDP");
    application a(argc, argv);
    //    MainWindow w;
    //    w.show();

    floatBall ball;
    ball.show();

//    StockKlineViewData b;

//    QString str=App->getCurrentTime();
//    QString str1=App->getLastMonthTime();
//    QString str2=App->getLastYearTime();
//    b.setData("002426",str2,str);
//    b.updateData();

//    stackStock cc;
//    cc.setData("sz002426");
//    StockCanvas x("1002624");
//    x.show();
    return a.exec();
}
