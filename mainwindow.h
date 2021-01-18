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

    void initDB();
    QByteArray getReply(QString surl);
    void sendData();
    void addGP(const QString & str);
    void removeGP(const QString & str);
private slots:
    void on_pushButton_clicked();
    void replyFinished(QNetworkReply *reply);
    void updateData();

    void on_comboBox_activated(const QString &arg1);

    void on_miniTable_doubleClicked(const QModelIndex &index);

    void on_miniTable_itemDoubleClicked(QTableWidgetItem *item);

    void on_normalBtn_clicked();

    void on_detailedBtn_clicked();

    void on_myBtn_clicked();

    void on_addMyBtn_clicked();

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *m_trayIcon{nullptr};
    QNetworkAccessManager *manager {nullptr};
    QNetworkReply *reply {nullptr};
    QMap <QString,DataGP> m_mGp;
    QTimer * m_timer{nullptr};
    QMutex* m_mutex{nullptr};
    QString m_currentZQ="sh";
//    QSqlDatabase m_db;
};

#endif // MAINWINDOW_H
