#include "mainwindow.h"
#include <application.h>

int main(int argc, char *argv[])
{
    application a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
