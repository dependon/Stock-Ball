#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMap>
#include <QSqlDatabase>
#include "data.h"
class QNetworkReply;
class QSystemTrayIcon;
class QNetworkAccessManager;
class QTimer;
class QMutex;
class QTableWidgetItem;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

//    void initDB();
//    QByteArray getReply(QString surl);
//    void sendData();
//    void addGP(const QString & str);
//    void removeGP(const QString & str);
//    void sendData2();
    void refreshNormalWidget();
    void refreshMyhaveWidget();
private slots:
    void on_pushButton_clicked();
//    void replyFinished(QNetworkReply *reply);
//    void replyFinished2(QNetworkReply *reply);
//    void updateData();

    void on_comboBox_activated(const QString &arg1);

    void on_miniTable_doubleClicked(const QModelIndex &index);

    void on_normalBtn_clicked();

    void on_detailedBtn_clicked();

    void on_myBtn_clicked();

    void on_addMyBtn_clicked();

    void slotDataGPsChange(MapdataGP map);
    void slotDataHaveGPsChange(MapdataHaveGP map);
    void slotDataAllDPChange(DataAllDP data);
private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *m_trayIcon{nullptr};
    QNetworkAccessManager *manager {nullptr};
    QNetworkAccessManager *manager2 {nullptr};
    QNetworkReply *reply {nullptr};
    QNetworkReply *reply2 {nullptr};
    QMap <QString,DataGP> m_mGp;
    QMap <QString,DataHaveGP> m_mMyGp;
    QTimer * m_timer{nullptr};

    DataAllDP m_myAllDP;
    QString m_currentZQ="sh";
    QString cureentInfo;
//    QSqlDatabase m_db;
};

#endif // MAINWINDOW_H
