#include "application.h"
#include <QDateTime>

application::application(int &argc, char **argv):
    QApplication(argc,argv)
{
    //    m_mutex=new QMutex();
    m_dataresovle = DataResovle::instance();
    m_signal=signalM::instance();
}

QString application::getCurrentTime()
{
    int year=QDateTime::currentDateTime().date().year();
    int month=QDateTime::currentDateTime().date().month();
    int day=QDateTime::currentDateTime().date().day();
    QString strEndyear=QString::number(year);
    QString strEndMonth=QString::number(month);
    QString strEndDay=QString::number(day);

    if(month <10 ){
        strEndMonth="0"+QString::number(month);
    }
    if(day <10 ){
        strEndDay="0"+QString::number(day);
    }

    QString number=strEndyear+strEndMonth+strEndDay;
    return number;
}

QString application::getLastMonthTime()
{
    int year=QDateTime::currentDateTime().date().year();
    int month=QDateTime::currentDateTime().date().month();
    int day=QDateTime::currentDateTime().date().day();
    if(month>1){
        month=month-1;
    }else {
        year=year-1;
        month=12;
    }
    QString stryear=QString::number(year);
    QString strMonth=QString::number(month);
    QString strDay=QString::number(day);

    if(month <10 ){
        strMonth="0"+QString::number(month);
    }
    if(day <10 ){
        strDay="0"+QString::number(day);
    }

    QString number=stryear+strMonth+strDay;
    return number;
}

QString application::getLastYearTime()
{
    int year=QDateTime::currentDateTime().date().year();
    int month=QDateTime::currentDateTime().date().month();
    int day=QDateTime::currentDateTime().date().day();
    year=year-1;
    QString stryear=QString::number(year);
    QString strMonth=QString::number(month);
    QString strDay=QString::number(day);

    if(month <10 ){
        strMonth="0"+QString::number(month);
    }
    if(day <10 ){
        strDay="0"+QString::number(day);
    }

    QString number=stryear+strMonth+strDay;
    return number;
}

