#ifndef ADDMONEYWIDGET_H
#define ADDMONEYWIDGET_H

#include <QDialog>
#include "data.h"
class QNetworkAccessManager;
class QNetworkReply;
namespace Ui {
class addMoneyWidget;
}

class addMoneyWidget : public QDialog
{
    Q_OBJECT

public:
    explicit addMoneyWidget(QWidget *parent = nullptr);
    ~addMoneyWidget();
    void setCodecData(QMap <QString,DataGP> gpMap);

    void addGP();

    void sendData();
private slots:
    void on_minecodecBtn_clicked();

    void on_noMineSearchBtn_clicked();

    void on_cancelBtn_clicked();

    void on_setBtn_clicked();

    void replyFinished(QNetworkReply *reply);
private:
    Ui::addMoneyWidget *ui;

    QNetworkAccessManager *manager {nullptr};
    QNetworkReply *reply {nullptr};
    DataGP currentData;

};

#endif // ADDMONEYWIDGET_H
