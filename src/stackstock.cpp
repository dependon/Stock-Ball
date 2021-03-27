#include "stackstock.h"
#include "ui_stackstock.h"
#include "stockView/stockcanvas.h"
#include "stockKlineView/stockklineviewdata.h"
#include "application.h"

stackStock::stackStock(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::stackStock)
{
    ui->setupUi(this);
}

stackStock::~stackStock()
{
    delete ui;
}

void stackStock::setData(QString code)
{
    if (!code.isEmpty()) {
        QString codeNum = code;
        QString codec = code.replace("sz", "1");
        codec = codec.replace("sh", "0");
        m_codec = codec;
        m_codeNum = codeNum;
//        codeNum=codeNum.replace("sz","");
//        m_codeNum=codeNum.replace("sh","");
        char  *chSecID;
        QByteArray baSecID = codec.toLatin1(); // must
        chSecID = baSecID.data();
        if (!m_Stock) {
            m_Stock = new StockCanvas(codec);
            m_Stock->setMinimumSize(400, 300);
        } else {
            m_Stock->setIDandTime(chSecID);
        }
        m_Stock->setWindowTitle(codec);
//        m_Stock->show();


        if (!m_KlineDay) {
            m_KlineDay = new StockKlineViewData();
//            m_KlineMonth->setData(m_codeNum,App->getLastMonthTime(),App->getCurrentTime());
            m_KlineDay->setData(m_codeNum, DAYKLINE);
        } else {
//            m_KlineMonth->setData(m_codeNum,App->getLastMonthTime(),App->getCurrentTime());
            m_KlineDay->setData(m_codeNum, DAYKLINE);

        }
        m_KlineDay->setWindowTitle(codec);

        if (!m_KlineWeek) {
            m_KlineWeek = new StockKlineViewData();
//            m_KlineYear->setData(m_codeNum,App->getLastYearTime(),App->getCurrentTime());
            m_KlineWeek->setData(m_codeNum, WEEKKLINE);

        } else {
//            m_KlineYear->setData(m_codeNum,App->getLastYearTime(),App->getCurrentTime());
            m_KlineWeek->setData(m_codeNum, WEEKKLINE);
        }
        m_KlineWeek->setWindowTitle(codec);

        if (!m_KlineMonth) {
            m_KlineMonth = new StockKlineViewData();
//            m_KlineYear->setData(m_codeNum,App->getLastYearTime(),App->getCurrentTime());
            m_KlineMonth->setData(m_codeNum, MONTHKLINE);

        } else {
//            m_KlineYear->setData(m_codeNum,App->getLastYearTime(),App->getCurrentTime());
            m_KlineMonth->setData(m_codeNum, MONTHKLINE);
        }
        m_KlineMonth->setWindowTitle(codec);
//        m_KlineYear->show();
    }
    ui->vday->addWidget(m_Stock);

    ui->dayK->addWidget(m_KlineDay);
    ui->weekK->addWidget(m_KlineWeek);
    ui->monthK->addWidget(m_KlineMonth);

    this->show();
    this->setWindowTitle(m_codec);
//    m_Stock=new StockCanvas();
//    m_Stock->setIDandTime("0000001");
//    m_Stock->setMinimumHeight(300);
//    m_Stock->setStatus(ViewStatus::NOLINETIP);
}
