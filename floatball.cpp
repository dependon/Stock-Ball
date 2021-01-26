#include "floatball.h"
#include <QLabel>
#include <QMenu>
#include <QCoreApplication>
#include <QSystemTrayIcon>
#include "signalm.h"
#include "mainwindow.h"
#include <QPainter>

floatBall::floatBall(QWidget *parent):
    QWidget(parent)
{
    setStyleSheet("QLabel { color:white; padding:2px; border:1px solid white; border-radius:15px; }");
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAutoFillBackground(true);
    setFixedSize(QSize(100,50));
    QString  SS = QString("background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0,"
                          "stop:0 rgba(255,0,0,200), stop:%1 rgba(255,0,0,200),"
                          "stop:%2 rgba(0,0,0,200), stop:1 rgba(0,0,0,200));")
            .arg(0*1.0/100-0.001)
            .arg(0*1.0/100);

    QFont font;
    font.setPointSize(10);
    //qDebug() << SS;
    m_label =new QLabel(this);
    m_label->setStyleSheet(SS);
    m_label->setText("日收 0.00 元\n日率 0.00 %");
    m_label->setFont(font);

    m_floatShowLabel=new QLabel();
    m_floatShowLabel->setFixedWidth(200);
    m_floatShowLabel->setWindowFlags(Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);
    font.setPointSize(10);
    m_floatShowLabel->setFont(font);
    m_floatShowLabel->setAlignment(Qt::AlignVCenter);
    m_floatShowLabel->setStyleSheet("QLabel { padding:2px; color:white; background-color:rgba(0,0,0,200); border-radius:15px; }");//无效

    m_trayIcon=new QSystemTrayIcon(this);
    m_trayIcon->setToolTip("test");
    m_trayIcon->show();

    m_mainWindow =new MainWindow();
    initConnect();
    initLeftMenu();


}

void floatBall::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        relativePos = pos()- event->globalPos();
    }
}

void floatBall::mouseMoveEvent(QMouseEvent *event)
{
    setCursor(Qt::ClosedHandCursor);
    move(event->globalPos() + relativePos);
    m_floatShowLabel->move(x() - m_floatShowLabel->width()/2, y() - m_floatShowLabel->height() - 5);
}

void floatBall::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    setCursor(Qt::ArrowCursor);
}
void floatBall::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    m_floatShowLabel->move(x() - m_floatShowLabel->width()/2, y()-m_floatShowLabel->height() - 5);
    m_floatShowLabel->show();
}

void floatBall::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    m_floatShowLabel->hide();
}

void floatBall::initConnect()
{
    connect(signalM::instance(),&signalM::sendDataGPsChange,this,&floatBall::slotDataGPsChange);
    connect(signalM::instance(),&signalM::sendDataHaveGPsChange,this,&floatBall::slotDataHaveGPsChange);
    connect(signalM::instance(),&signalM::sendDataAllDPChange,this,&floatBall::slotDataAllDPChange);
}

void floatBall::initLeftMenu()
{
    m_leftMenu=new QMenu();


    QAction * settingAction=new QAction("设置",m_leftMenu);
    connect(settingAction, &QAction::triggered, this,[=]{
        m_mainWindow->show();
    });
    m_leftMenu->addAction(settingAction);

    m_hideAction=new QAction("隐藏",m_leftMenu);
    connect(m_hideAction, &QAction::triggered, this,[=]{
        if(m_label->isVisible()){
            m_label->hide();
            m_showAction->setText("显示");
            m_hideAction->setText("显示");
        }
        else {
            m_label->show();
            m_showAction->setText("隐藏");
            m_hideAction->setText("隐藏");
        }
    });
    m_leftMenu->addAction(m_hideAction);

    QAction * exitaction=new QAction("退出",m_leftMenu);
    connect(exitaction, SIGNAL(triggered()), qApp, SLOT(quit()));
    m_leftMenu->addAction(exitaction);


    m_trayMenu=new QMenu();
    QAction * settingAction2=new QAction("设置",m_leftMenu);
    connect(settingAction2, &QAction::triggered, this,[=]{
        m_mainWindow->show();
    });
    m_trayMenu->addAction(settingAction2);



    m_showAction=new QAction("隐藏",m_leftMenu);
    connect(m_showAction, &QAction::triggered, this,[=]{
        if(m_label->isVisible()){
            m_label->hide();
            m_showAction->setText("显示");
            m_hideAction->setText("显示");
        }
        else {
            m_label->show();
            m_showAction->setText("隐藏");
            m_hideAction->setText("隐藏");
        }
    });
    m_trayMenu->addAction(m_showAction);

    QAction * exitaction2=new QAction("退出",m_leftMenu);
    connect(exitaction2, SIGNAL(triggered()), qApp, SLOT(quit()));
    m_trayMenu->addAction(exitaction2);

    m_trayIcon->setContextMenu(m_trayMenu);

}

void floatBall::slotDataGPsChange(MapdataGP map)
{
    QMap <QString ,DataGP> mapdata=map.map;
}

void floatBall::slotDataHaveGPsChange(MapdataHaveGP map)
{
    QString floatShowStr;
    QMap <QString ,DataHaveGP> mapdata=map.map;

    DataAllDP m_myAllDP=DataAllDP();
    int index1=0;
    DataHaveGP allgp;
    for(auto gp:mapdata){
        allgp.haveNum+=gp.haveNum;
        allgp.todaySY+=gp.todaySY;
        allgp.historySY+=gp.historySY;
        allgp.payallPrice+=gp.payallPrice;
        allgp.currentPrice+=gp.currentPrice;
        allgp.yesterDayPrice+=gp.yesterDayPrice;
        allgp.currentallPrice+=gp.currentallPrice;
        double historySyl=100*gp.historySY/gp.payallPrice;
        QString historyL=QString::number(historySyl,'g',3)+"%";
        double todayyl=100*(gp.currentPrice-gp.yesterDayPrice)/gp.yesterDayPrice;
        QString todayL=QString::number(todayyl,'g',3)+"%";
        m_myAllDP.payallPrice+=gp.payallPrice;
        m_myAllDP.currentallPrice+=gp.currentallPrice;
        m_myAllDP.todaySY+=gp.todaySY;
        index1++;
        if(gp.todaySY>0){
            floatShowStr+=gp.name+":↑"+QString::number(gp.todaySY)+"元 "+todayL+"\n";
        }
        else {
            floatShowStr+=gp.name+":↓"+QString::number(gp.todaySY)+"元 "+todayL+"\n";
        }
    }
    double historySyl=100*allgp.historySY/allgp.payallPrice;
    QString historyL=QString::number(historySyl,'g',3)+"%";

    if(historySyl>0){
        //        ui->myTable->item(index1,5)->setTextColor(QColor("white"));
        //        ui->myTable->item(index1,6)->setTextColor(QColor("white"));
        //        ui->myTable->item(index1,5)->setBackground(QColor("red"));
        //        ui->myTable->item(index1,6)->setBackground(QColor("red"));
    }
    else {
        //        ui->myTable->item(index1,5)->setTextColor(QColor("white"));
        //        ui->myTable->item(index1,6)->setTextColor(QColor("white"));
        //        ui->myTable->item(index1,5)->setBackground(QColor("green"));
        //        ui->myTable->item(index1,6)->setBackground(QColor("green"));
    }
    double todayyl=100*(allgp.todaySY)/allgp.currentallPrice;
    QString todayL=QString::number(todayyl,'g',3)+"%";
    QString SS;
    if(todayyl<-10 ||todayyl>10){
        SS = QString("background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0,"
                     "stop:0 rgba(0,200,0,200), stop:%1 rgba(0,200,0,200),"
                     "stop:%2 rgba(0,0,0,200), stop:1 rgba(0,0,0,200));")
                .arg(0*1.0/100-0.001)
                .arg(0*1.0/100);
    }
    else if(todayyl<0){
        SS = QString("background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0,"
                     "stop:0 rgba(0,200,0,200), stop:%1 rgba(0,200,0,200),"
                     "stop:%2 rgba(0,0,0,200), stop:1 rgba(0,0,0,200));")
                .arg(-todayyl*10.0/100-0.001)
                .arg(-todayyl*10.0/100);
    }
    else{
        SS = QString("background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0,"
                     "stop:0 rgba(255,0,0,200), stop:%1 rgba(255,0,0,200),"
                     "stop:%2 rgba(0,0,0,200), stop:1 rgba(0,0,0,200));")
                .arg(todayyl*10.0/100-0.001)
                .arg(todayyl*10.0/100);
    }
    m_label->setStyleSheet(SS);
    QString cureentInfo="日收:" +QString::number(allgp.todaySY)+"元" +"\n" +"日率:"+ todayL;
    m_label->setText(cureentInfo);
    m_trayIcon->setToolTip(cureentInfo);

    floatShowStr+="日收益:" +QString::number(allgp.todaySY)+"元" +" " +"日利率:"+ todayL;
    m_floatShowLabel->setText(floatShowStr);

    //修改托盘图标
    QPixmap pixmap(128,128);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPen pen;
    if (todayyl<0){
        pen.setColor(Qt::green);
    }
    //↑
    else{
        pen.setColor(Qt::red);
    }

    pen.setWidth(10);
    painter.setPen(pen);
//    painter.drawArc(pixmap.rect().adjusted(10,10,-10,-10), 0, 360*16*todayyl/100);
    QFont font;
    font.setPointSize(100);
    painter.setFont(font);
    if (todayyl<0){
        painter.drawText(pixmap.rect(), Qt::AlignCenter, "↓");
    }
    else{
        painter.drawText(pixmap.rect(), Qt::AlignCenter, "↑");
    }
    QIcon icon(pixmap);
    m_trayIcon->setIcon(icon);
}

void floatBall::slotDataAllDPChange(DataAllDP data)
{

}

void floatBall::contextMenuEvent(QContextMenuEvent *event)
{
    m_leftMenu->exec(QCursor::pos());
    event->accept();
}

void floatBall::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(m_mainWindow){
        m_mainWindow->show();
    }
}

