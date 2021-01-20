#include "signalm.h"

signalM *signalM::m_signal =nullptr;
signalM *signalM::instance()
{
    if (! m_signal) {
        m_signal = new signalM();
    }

    return m_signal;
}

signalM::signalM(QObject *parent) : QObject(parent)
{

}
