#ifndef FLOATBALL_H
#define FLOATBALL_H

#include <QWidget>
#include <QMouseEvent>
#include "data.h"
class QMenu;
class QLabel;
class MainWindow;
class QSystemTrayIcon;
class floatBall: public QWidget
{
    Q_OBJECT
public:
    floatBall(QWidget *parent = nullptr );
    void initConnect();
    void initLeftMenu();
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    void mouseDoubleClickEvent(QMouseEvent* event);

public slots:
    void slotDataGPsChange(MapdataGP map);
    void slotDataHaveGPsChange(MapdataHaveGP map);
    void slotDataAllDPChange(DataAllDP data);

private:
    QLabel *m_label{nullptr};
    QPoint relativePos;
    QMenu *m_leftMenu{nullptr};
    QMenu *m_trayMenu{nullptr};
    MainWindow * m_mainWindow{nullptr};
    QSystemTrayIcon *m_trayIcon{nullptr};
    QAction *m_showAction{nullptr};
    QAction *m_hideAction{nullptr};
};

#endif // FLOATBALL_H
