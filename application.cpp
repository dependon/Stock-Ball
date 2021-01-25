#include "application.h"

application::application(int &argc, char **argv):
    QApplication(argc,argv)
{
    //    m_mutex=new QMutex();
    m_dataresovle = DataResovle::instance();
    m_signal=signalM::instance();
}

