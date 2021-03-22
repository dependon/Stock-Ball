#ifndef APPLICATION_H
#define APPLICATION_H
#include <QApplication>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QMutex>
#include "dataresovle.h"
#include "signalm.h"
#define App (static_cast<application*>(QCoreApplication::instance()))
class application:public QApplication
{
    Q_OBJECT
public:
    application(int& argc, char **argv);

    QString getCurrentTime();
    QString getLastMonthTime();
    QString getLastYearTime();
    signalM *m_signal{nullptr};
    DataResovle *m_dataresovle{nullptr};
};

#endif // APPLICATION_H
