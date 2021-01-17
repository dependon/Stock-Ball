#ifndef ADDMONEYWIDGET_H
#define ADDMONEYWIDGET_H

#include <QDialog>
#include "data.h"
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

private slots:
    void on_minecodecBtn_clicked();

    void on_noMineSearchBtn_clicked();

    void on_cancelBtn_clicked();

    void on_setBtn_clicked();

private:
    Ui::addMoneyWidget *ui;
};

#endif // ADDMONEYWIDGET_H
