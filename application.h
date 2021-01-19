#ifndef APPLICATION_H
#define APPLICATION_H
#include <QApplication>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QMutex>
#define App (static_cast<application*>(QCoreApplication::instance()))
class application:public QApplication
{
    Q_OBJECT
public:
    application(int& argc, char **argv);
    QSqlDatabase m_db;
    QMutex* m_mutex{nullptr};
};

#endif // APPLICATION_H
