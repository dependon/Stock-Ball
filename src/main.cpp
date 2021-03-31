#include "mainwindow.h"
#include "application.h"
#include "data.h"
#include "floatball.h"
#include "stackstock.h"
#include "stockKlineView/stockklineviewdata.h"

#include <QMetaType>
#include <QDateTime>
#include <QDir>
#include <QDebug>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <QMap>

//进程单例
bool checkOnly()
{
    //single
    QString userName = QDir::homePath().section("/", -1, -1);
    std::string path = ("/home/" + userName + "/.config/stockball/").toStdString();
    QDir tdir(path.c_str());
    if (!tdir.exists()) {
        bool ret =  tdir.mkpath(path.c_str());
        qDebug() << ret ;
    }

    path += "single";
    int fd = open(path.c_str(), O_WRONLY | O_CREAT, 0644);
    int flock = lockf(fd, F_TLOCK, 0);

    if (fd == -1) {
        perror("open lockfile/n");
        return false;
    }
    if (flock == -1) {
        perror("lock file error/n");
        return false;
    }
    return true;
}
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

    if(!checkOnly()){
        return 0;
    }
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
