#ifndef APPLICATION_H
#define APPLICATION_H
#include <QApplication>
#include <QCoreApplication>
#define App (static_cast<Application*>(QCoreApplication::instance()))
class application:public QApplication
{
    Q_OBJECT
public:
    application(int& argc, char **argv);
};

#endif // APPLICATION_H
