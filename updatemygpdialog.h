#ifndef UPDATEMYGPDIALOG_H
#define UPDATEMYGPDIALOG_H

#include <QDialog>
#include "data.h"
namespace Ui {
class updateMyGpDialog;
}

class updateMyGpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit updateMyGpDialog(DataHaveGP map,QWidget *parent = nullptr);
    ~updateMyGpDialog();

private slots:
    void on_okBtn_clicked();

    void on_cancelBtn_clicked();

    void on_totalEdit_textChanged(const QString &arg1);

    void on_numEdit_textChanged(const QString &arg1);

    void on_eveEdit_textChanged(const QString &arg1);


private:
    Ui::updateMyGpDialog *ui;
    DataHaveGP m_map;
};

#endif // UPDATEMYGPDIALOG_H
