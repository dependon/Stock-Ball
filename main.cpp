#include "mainwindow.h"
#include <application.h>
#include "data.h"
#include <QMetaType>

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
    MainWindow w;
    w.show();

    return a.exec();
}
