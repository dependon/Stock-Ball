#ifndef FLOATBALL_H
#define FLOATBALL_H

#include <QWidget>
#include <QMouseEvent>
#include "data.h"
class QMenu;
class QLabel;
class MainWindow;
class floatBall: public QWidget
{
    Q_OBJECT
public:
    floatBall(QWidget *parent = nullptr );
    QLabel *m_label{nullptr};

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void initConnect();
    void initLeftMenu();

    void slotDataGPsChange(MapdataGP map);
    void slotDataHaveGPsChange(MapdataHaveGP map);
    void slotDataAllDPChange(DataAllDP data);
    void contextMenuEvent(QContextMenuEvent *event);
    void mouseDoubleClickEvent(QMouseEvent* event);

    QPoint relativePos;
    QMenu *m_leftMenu{nullptr};
    MainWindow * m_mainWindow{nullptr};
};

#endif // FLOATBALL_H
