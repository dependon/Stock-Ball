#ifndef KLINEGRID_H
#define KLINEGRID_H

#include "autogrid.h"
#include "datafile.h"

#include <QPoint>
#include "showdetail.h"
#include <QString>


class QMutex;
class ShowDetail;

class KLineGrid : public AutoGrid
{
    Q_OBJECT

public:
    explicit KLineGrid(QWidget *parent = 0);
    void virtual paintEvent(QPaintEvent* event);
    void virtual keyPressEvent(QKeyEvent* event);
    void virtual mouseMoveEvent(QMouseEvent* event);
    void virtual mousePressEvent(QMouseEvent* event);
    void virtual resizeEvent(QResizeEvent* event);
    ~KLineGrid();



    bool readData(QString strFile);
    bool readData(std::vector<KLine> datas);
    void initial();
    void drawLine();
    void getIndicator();
    void drawYtick();
    void drawKline();


    //键盘按下后画的十字线
    void drawCross();
    void drawCrossVerLine();
    void drawCrossHorLine();
    void drawTips();

    //键盘没按下画的十字线
    void drawCross2();
    void drawMouseMoveCrossVerLine();
    void drawMouseMoveCrossHorLine();
    void drawTips2();



    //画均线
    void drawAverageLine(int day);

private:
    DataFile mDataFile;
    ShowDetail* mShowDrtail{nullptr};


    //画k线的起始日期和终止日期
    int beginDay{0};
    int endDay{0};
    int totalDay{0};
    int currentDay{0};


    //当前要画的k线日期中的最高价，最低价，最大成交量
    double highestBid{0.0};
    double lowestBid{0.0};
    double maxVolume{0.0};

    //x轴和y轴的缩放比
    double xscale{0.0};
    double yscale{0.0};

    //是否显示十字线
    bool bCross = false;


    //鼠标位置
    QPoint mousePoint;

    //画笔的线宽
    int lineWidth;


    //键盘是否按下
    bool isKeyDown = false;



    //是否画均线
    bool isDrawAverageLine = true;

    QMutex *m_mutex;


signals:



};

#endif // KLINEGRID_H
