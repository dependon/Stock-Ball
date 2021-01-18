#ifndef APPLICATION_H
#define APPLICATION_H
#include <QApplication>
#include <QCoreApplication>
#include <QSqlDatabase>
#define App (static_cast<application*>(QCoreApplication::instance()))
class application:public QApplication
{
    Q_OBJECT
public:
    application(int& argc, char **argv);
    QSqlDatabase m_db;
};

#endif // APPLICATION_H
