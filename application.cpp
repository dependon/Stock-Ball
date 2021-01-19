#include "application.h"

application::application(int &argc, char **argv):
      QApplication(argc,argv)
{
    m_mutex=new QMutex();
}
