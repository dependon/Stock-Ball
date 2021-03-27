#ifndef STACKSTOCK_H
#define STACKSTOCK_H

#include <QWidget>
class StockCanvas;
class StockKlineViewData;
namespace Ui {
class stackStock;
}

class stackStock : public QWidget
{
    Q_OBJECT

public:
    explicit stackStock(QWidget *parent = nullptr);
    ~stackStock();
    void setData(QString code);

private:
    Ui::stackStock *ui;

    StockCanvas *m_Stock{nullptr};

    StockKlineViewData *m_KlineDay{nullptr};
    StockKlineViewData *m_KlineWeek{nullptr};
    StockKlineViewData *m_KlineMonth{nullptr};

    QString m_codec;
    QString m_codeNum;
};

#endif // STACKSTOCK_H
