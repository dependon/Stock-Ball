#include "stockcanvas.h"
#include <QDebug>
#include <QPen>
#include <QPainter>
#include <QDateTime>
#include <QTimer>

StockCanvas::StockCanvas(const QString & SecID,const QString & szDate,QWidget *parent) : QWidget(parent)
{
    m_szSecID = SecID;
    m_szDate = szDate;
    //默认铺满窗口并设置背景色为黑色
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Window,QColor("#000000"));
    this->setPalette(palette);
    //开启鼠标追踪
    setMouseTracking(true);

    m_fsjl= new stockViewData();
    m_fsjl->setData(m_szSecID,m_szDate);

    connect(m_fsjl,&stockViewData::refreashView,this,[=]{
        update();
    });

}

void StockCanvas::setIDandTime(const QString &SecID, const QString &szDate)
{
     m_szSecID = SecID;
     m_szDate = szDate;
     m_fsjl->setData(m_szSecID,m_szDate);
}

void StockCanvas::setStatus(ViewStatus status)
{
    m_viewStatus=status;
}

void StockCanvas::paintEvent(QPaintEvent *event)
{
    DrawBK();
    DrawMouseLine();
    DrawFSJL(m_szSecID,m_szDate);
}

void StockCanvas::resizeEvent(QResizeEvent *event)
{
    m_windowHeight = this->height();
    m_windowWidth = this->width();
}


void StockCanvas::DrawBorder()
{

    //画外边框

    QPainter painter(this);
    QPen     pen;
    pen.setColor(QColor("#FF0000"));
    painter.setPen(pen);


    painter.drawLine(0+BORDER_SIZE,0+BORDER_SIZE,
                     0+BORDER_SIZE,m_windowHeight-BORDER_SIZE); //left |
    painter.drawLine(0+BORDER_SIZE,0+BORDER_SIZE,
                     m_windowWidth-BORDER_SIZE,0+BORDER_SIZE); //top -
    painter.drawLine(m_windowWidth-BORDER_SIZE,0+BORDER_SIZE,
                     m_windowWidth-BORDER_SIZE,m_windowHeight-BORDER_SIZE); //right |
    painter.drawLine(0+BORDER_SIZE,m_windowHeight-BORDER_SIZE,
                     m_windowWidth-BORDER_SIZE,m_windowHeight-BORDER_SIZE); //bottom -

}

void StockCanvas::DrawBK()
{

    QPainter painter(this);
    QPen     pen;
    pen.setColor(QColor("#FF0000"));
    painter.setPen(pen);


    //DrawBorder();

    //画背景表格

    painter.drawLine(0+BORDER_SIZE+COORDINATE_X1,0+BORDER_SIZE+COORDINATE_Y1,
                     0+BORDER_SIZE+COORDINATE_X1,m_windowHeight-BORDER_SIZE-COORDINATE_Y2);
    painter.drawLine(0+BORDER_SIZE+COORDINATE_X1,0+BORDER_SIZE+COORDINATE_Y1,
                     m_windowWidth-BORDER_SIZE-COORDINATE_X2,0+BORDER_SIZE+COORDINATE_Y1);
    painter.drawLine(m_windowWidth-BORDER_SIZE-COORDINATE_X2,0+BORDER_SIZE+COORDINATE_Y1,
                     m_windowWidth-BORDER_SIZE-COORDINATE_X2,m_windowHeight-BORDER_SIZE-COORDINATE_Y2);
    painter.drawLine(0+BORDER_SIZE+COORDINATE_X1,m_windowHeight-BORDER_SIZE-COORDINATE_Y2,
                     m_windowWidth-BORDER_SIZE-COORDINATE_X2,m_windowHeight-BORDER_SIZE-COORDINATE_Y2);


    yInterval = (double)( (m_windowHeight-BORDER_SIZE-COORDINATE_Y2)- (0+BORDER_SIZE+COORDINATE_Y1) ) / 15 + 0.5; //三部分，每部分5个大格子
    pen.setColor(QColor("#FF0000"));
    for( int i=1,j=yInterval; i < 15; i++,j+=yInterval)
    {
        if( i % 5 != 0 )
        {
            pen.setStyle(Qt::DashLine);
            painter.setPen(pen);
        }
        else
        {
            pen.setStyle(Qt::SolidLine);
            painter.setPen(pen);
        }
        painter.drawLine(0+BORDER_SIZE+COORDINATE_X1,0+BORDER_SIZE+COORDINATE_Y1+j,
                         m_windowWidth-BORDER_SIZE-COORDINATE_X2,0+BORDER_SIZE+COORDINATE_Y1+j);
    }

    xInterval = (double)( (m_windowWidth-BORDER_SIZE-COORDINATE_X2)- (0+BORDER_SIZE+COORDINATE_X1) ) / 8 + 0.5;
    for( int x=1,y=xInterval; x < 8; x++,y+=xInterval)
    {

        if( x % 2 != 0 )
        {
            pen.setStyle(Qt::DashLine);
            painter.setPen(pen);
        }
        else
        {
            pen.setStyle(Qt::SolidLine);
            painter.setPen(pen);
        }
        painter.setPen(pen);
        painter.drawLine(0+BORDER_SIZE+COORDINATE_X1+y,0+BORDER_SIZE+COORDINATE_Y1,
                         0+BORDER_SIZE+COORDINATE_X1+y,m_windowHeight-BORDER_SIZE-COORDINATE_Y2);
    }
}

void StockCanvas:: mouseMoveEvent(QMouseEvent* event)
{
    //设定鼠标移动范围
    mousePoint = QPoint(event->pos());
    m_xGridMin = BORDER_SIZE+COORDINATE_X1;
    m_xGridMax = m_windowWidth - (BORDER_SIZE+COORDINATE_X1);
    m_yGridMin = BORDER_SIZE+COORDINATE_Y1;
    m_yGridMax = m_windowHeight-BORDER_SIZE-COORDINATE_Y2;
    if(mousePoint.y() < m_yGridMin || mousePoint.y() > m_yGridMax){
        return;
    }
    if(mousePoint.x() < m_xGridMin || mousePoint.x() > m_xGridMax){
        return;
    }
    update();
}

//画跟随鼠标的十字线
void StockCanvas::DrawMouseLine()
{
    if(m_viewStatus!=ViewStatus::NOLINETIP){
        QLineF linex(0+BORDER_SIZE+COORDINATE_X1,mousePoint.y(),this->width()-(BORDER_SIZE+COORDINATE_X1),mousePoint.y());
        QLineF liney(mousePoint.x(),BORDER_SIZE+COORDINATE_Y1,mousePoint.x(),this->height()-(BORDER_SIZE+COORDINATE_Y2));
        QPainter painter(this);
        QPen     pen;
        pen.setColor(QColor("#FFFFFF"));
        pen.setWidth(1);
        painter.setPen(pen);
        painter.drawLine(linex);
        painter.drawLine(liney);
        DrawTips();
    }
}



void StockCanvas::DrawFSJL(const QString& SecID,const QString&  szDate)
{
    if(m_fsjl->fsjl.count()<1){
        return;}


    QPainter painter(this);
    QPen     pen;
    pen.setColor(Qt::white);
    painter.setPen(pen);

    //画右侧涨跌百分比 0%
    yInterval  = (double)( (m_windowHeight-BORDER_SIZE-COORDINATE_Y2)- (0+BORDER_SIZE+COORDINATE_Y1) ) / 15 ;
    painter.drawText(QPoint(m_windowWidth-BORDER_SIZE-COORDINATE_X2+10,
                            0+BORDER_SIZE+COORDINATE_Y1+yInterval*5 +2),QString("0.00%"));

    //画左侧初始股票价格 0

    QString str;
    str.sprintf("%.2f",(float)m_fsjl->info.deal_Start / 1000 );
    painter.drawText(QPoint(0+BORDER_SIZE+COORDINATE_X1-TEXT_LENGTH,
                            0+BORDER_SIZE+COORDINATE_Y1+yInterval*5 +2),str);


    //根据读入的数据画出涨跌百分比和股票价格 先画上半部分

    pen.setColor(Qt::red);
    painter.setPen(pen);
    float Proportional;
    for( int i = 5; i > 0 ; i-- )
    {
        Proportional = (float)m_fsjl->info.deal_rate / 5  * i * 100;
        str.sprintf( "%.2f%", Proportional );
        painter.drawText(QPoint(m_windowWidth-BORDER_SIZE-COORDINATE_X2+10,
                                0+BORDER_SIZE+COORDINATE_Y1+yInterval*(5-i)+2),str);

        str.sprintf("%.2f",(float)m_fsjl->info.deal_Start / 1000 * (1+Proportional/100));
        painter.drawText(QPoint(0+BORDER_SIZE+COORDINATE_X1-TEXT_LENGTH,
                                0+BORDER_SIZE+COORDINATE_Y1+yInterval*(5-i)+2),str);
    }

    //画下半部分
    pen.setColor(Qt::green);
    painter.setPen(pen);
    for(int i=1 ; i <= 5 ; i++ )
    {
        Proportional = (float)m_fsjl->info.deal_rate / 5  * i * 100;
        str.sprintf( "%.2f%%", Proportional );
        painter.drawText(QPoint(m_windowWidth-BORDER_SIZE-COORDINATE_X2+10,
                                0+BORDER_SIZE+COORDINATE_Y1+yInterval*(5+i)),str);

        str.sprintf("%.2f",(float)m_fsjl->info.deal_Start / 1000 * (1-Proportional/100));
        painter.drawText(QPoint(0+BORDER_SIZE+COORDINATE_X1-TEXT_LENGTH,
                                0+BORDER_SIZE+COORDINATE_Y1+yInterval*(5+i)+2),str);
    }




    //画时间轴

    pen.setColor(Qt::red);
    painter.setPen(pen);
    for(int i=0 ; i < 9 ; i++)
    {
        double rateI =  (double)( (m_windowWidth-BORDER_SIZE-COORDINATE_X2)- (0+BORDER_SIZE+COORDINATE_X1) ) / 8 + 0.5;
        QPoint ft(BORDER_SIZE+COORDINATE_X2-10+i*(rateI),m_windowHeight-BORDER_SIZE-COORDINATE_Y2+12);
        QDateTime time = QDateTime::fromString("09:30","HH:mm");
        QDateTime newTime;
        if(i >=4){
            newTime = time.addSecs(1800*(i+3));
        }else if(i<4){
            newTime = time.addSecs(1800*i);
        }
        QString str = newTime.toString("HH:mm");
        painter.drawText(ft,str);
    }



    //画分时线
    pen.setColor(QColor("#FFFFFF"));
    pen.setWidth(1);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);

    int zeroX = 0+BORDER_SIZE+COORDINATE_X1;
    int zeroY = 0+BORDER_SIZE+COORDINATE_Y1+yInterval*5;

    float xInter = float(m_windowWidth - 2 * BORDER_SIZE - COORDINATE_X1 -COORDINATE_X2)/241;

    QPoint p1(zeroX,zeroY),p2(zeroX,zeroY);

//    for( int i=0; i < 241; i++ )
//    {
//        if( 240 == i ){
//            p2.setX(this->width() - BORDER_SIZE - COORDINATE_X1);
//        }else{
//            p2.setX(zeroX + i* xInter);
//        }
//        double tempRate = float(m_fsjl->fsjl[i].Deal - m_fsjl->info.deal_Start) / m_fsjl->info.deal_Start;
//        p2.setY( zeroY - tempRate / m_fsjl->info.deal_rate * 5* yInterval ) ;
//        painter.drawLine(p1,p2);
//        p1 = p2;
//    }
//    m_fsjl->fsjl
    int index=0;
    for( auto fj : m_fsjl->fsjl)
    {
        if( 240 == index ){
            p2.setX(this->width() - BORDER_SIZE - COORDINATE_X1);
        }else{
            p2.setX(zeroX + index* xInter);
        }
        double tempRate = float(fj.Deal - m_fsjl->info.deal_Start) / m_fsjl->info.deal_Start;
        p2.setY( zeroY - tempRate / m_fsjl->info.deal_rate * 5* yInterval ) ;
        //第一个位置画点，不画折线
        if(0==index){
            painter.drawPoint(p2);
        }else{
            painter.drawLine(p1,p2);
        }
        p1 = p2;
        index++;
    }



    //画分时线下方的成交量

    float factor = 5 * yInterval / m_fsjl->info.vol_Max;
//    for( int i=0; i < 241; i++ )
//    {
//        p2.setX(zeroX + i* xInter);
//        p1.setX(zeroX + i* xInter);
//        p1.setY(m_windowHeight - BORDER_SIZE - COORDINATE_Y2);
//        p2.setY(BORDER_SIZE + COORDINATE_Y2 + 15*yInterval - m_fsjl->fsjl[i].Vol * factor);
//        painter.drawLine(p1,p2);
//    }
    int inFj=0;
    for( auto fj: m_fsjl->fsjl )
    {
        p2.setX(zeroX + inFj* xInter);
        p1.setX(zeroX + inFj* xInter);
        p1.setY(m_windowHeight - BORDER_SIZE - COORDINATE_Y2);
        p2.setY(BORDER_SIZE + COORDINATE_Y2 + 15*yInterval - fj.Vol * factor);
        painter.drawLine(p1,p2);
        inFj++;
    }




    //画左侧提示框

    DrawTips();

}

void StockCanvas::setLSpace(QString &str, int n)
{
    for( int i=0 ;i<n; i++ )
    {
        str.push_front(" ");
    }
}



void StockCanvas::DrawTips()
{
    if(m_viewStatus!=ViewStatus::NOLINETIP){
        double temp = mousePoint.y() - BORDER_SIZE - COORDINATE_Y1 - 5* yInterval;
        double y_val = - temp * (m_fsjl->info.deal_Start *  m_fsjl->info.deal_rate) / (5* yInterval) + m_fsjl->info.deal_Start;

        double y_valPCT = ( y_val  - m_fsjl->info.deal_Start ) / m_fsjl->info.deal_Start;


        y_val/= 1000;
        y_valPCT*= 100;

        //两侧价格和百分比显示

        int iTipsWidth  = 60;
        int iTipsHeight = 20;
        QRect rect(0+BORDER_SIZE+COORDINATE_X1-TEXT_LENGTH-15,
                   mousePoint.y() - iTipsHeight / 2 ,iTipsWidth,iTipsHeight);
        QPainter painter(this);
        QPen     pen;
        pen.setColor(Qt::white);



        if( y_valPCT > 0)
        {
            QBrush brush(Qt::red);
            painter.setBrush(brush);
        }
        else
        {
            QBrush brush(Qt::blue);
            painter.setBrush(brush);
        }


        pen.setWidth(1);
        painter.setPen(pen);
        painter.drawRect(rect);
        QString text;
        text.sprintf("%.2f",y_val);
        painter.drawText(rect,Qt::AlignCenter,text);


        QRect rect2(m_windowWidth - BORDER_SIZE - COORDINATE_X2,
                    mousePoint.y() - iTipsHeight / 2 ,iTipsWidth,iTipsHeight);
        painter.drawRect(rect2);
        text.sprintf("%.2f",y_valPCT);
        painter.drawText(rect2,Qt::AlignCenter,text);




        //底部时间显示

        double temp2 = float( mousePoint.x() - BORDER_SIZE - COORDINATE_X1 ) / (m_windowWidth - 2*BORDER_SIZE - COORDINATE_X1 - COORDINATE_X2 ) * 4;
        if( temp2 > 2)
            temp2 += 1.5;


        QDateTime time = QDateTime::fromString("09:30","HH:mm");
        QDateTime newTime;
        newTime = time.addSecs(temp2 * 60 * 60 );
        text = newTime.toString("HH:mm");



        QBrush brush(Qt::red);
        painter.setBrush(brush);
        //    painter.setPen(pen);
        QRect rect3( mousePoint.x() - iTipsWidth/2 , m_windowHeight - BORDER_SIZE - COORDINATE_Y2,
                     iTipsWidth , iTipsHeight);
        painter.drawRect(rect3);
        painter.drawText(rect3,text);
    }

}






















