#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QSystemTrayIcon>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTextCodec>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QMutex>
#include <QMutexLocker>
#include <QColor>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QFileInfo>
#include <QLabel>

#include <application.h>
#include <addmoneywidget.h>
#include "signalm.h"
#include "updatemygpdialog.h"
#include "stockView/stockcanvas.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //    m_trayIcon=new QSystemTrayIcon();
    //    m_trayIcon->setToolTip("test");
    //    m_trayIcon->show();

    //    ui->normalFrame->hide();
    ui->miniTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->myTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(signalM::instance(),&signalM::sendDataGPsChange,this,&MainWindow::slotDataGPsChange);
    connect(signalM::instance(),&signalM::sendDataHaveGPsChange,this,&MainWindow::slotDataHaveGPsChange);
    connect(signalM::instance(),&signalM::sendDataAllDPChange,this,&MainWindow::slotDataAllDPChange);
    m_shStock=new StockCanvas();
    m_shStock->setIDandTime("0000001");
    m_shStock->setMinimumHeight(300);
    m_szStock=new StockCanvas();
    m_szStock->setIDandTime("1399001");
    m_szStock->setMinimumHeight(300);
    m_cyStock=new StockCanvas();
    m_cyStock->setIDandTime("1399006");
    m_cyStock->setMinimumHeight(300);

    m_shLabel=new QLabel("上证指数");
    m_szLabel=new QLabel("深圳成指");
    m_cyLabel=new QLabel("创业板指");
    ui->shlayout->addWidget(m_shLabel);
    ui->szlayout->addWidget(m_szLabel);
    ui->cylayout->addWidget(m_cyLabel);
    ui->shlayout->addWidget(m_shStock);
    ui->szlayout->addWidget(m_szStock);
    ui->cylayout->addWidget(m_cyStock);
    initLeftMenu();
    setWindowTitle("配置界面");

}

MainWindow::~MainWindow()
{
    m_shStock->deleteLater();
    m_shStock=nullptr;
    m_szStock->deleteLater();
    m_szStock=nullptr;
    m_cyStock->deleteLater();
    m_cyStock=nullptr;
    m_szLabel->deleteLater();
    m_szLabel=nullptr;
    m_shLabel->deleteLater();
    m_shLabel=nullptr;
    m_cyLabel->deleteLater();
    m_cyLabel=nullptr;

    delete ui;
}

void MainWindow::initLeftMenu()
{
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->miniTable->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->myTable->setContextMenuPolicy(Qt::CustomContextMenu);
    m_normalLeftMenu=new QMenu(this);
    m_myLeftMenu=new QMenu(this);

    QAction *removeNoraml=new QAction("删除",m_normalLeftMenu);
    connect(removeNoraml,&QAction::triggered,this,[=]{
        if(ui->miniTable->currentRow()>=0){
            QString code=ui->miniTable->item(ui->miniTable->currentRow(),0)->data(1).toString();
            if(!code.isEmpty()){
                signalM::instance()->sendremoveGP(code);
            }
        }
        if(ui->tableWidget->currentRow()>=0){
            QString code=ui->tableWidget->item(ui->tableWidget->currentRow(),1)->data(0).toString();
            if(!code.isEmpty()){
                signalM::instance()->sendremoveGP(code);
            }
        }
    });
    m_normalLeftMenu->addAction(removeNoraml);

    QAction *editMy=new QAction("配置",m_myLeftMenu);
    connect(editMy,&QAction::triggered,this,[=]{
        if(ui->myTable->currentRow()>=0){
            QTableWidgetItem *item=ui->myTable->item(ui->myTable->currentRow(),0);
            QString key=item->data(1).toString();
            DataHaveGP haveGp=m_mMyGp.value(key);
            if(!haveGp.codec.isEmpty()){
                updateMyGpDialog dialog(haveGp);
                dialog.exec();
            }
        }
    });

    QAction *removeMy=new QAction("删除",m_myLeftMenu);
    connect(removeMy,&QAction::triggered,this,[=]{
        if(ui->myTable->currentRow()>=0){
            QString code=ui->myTable->item(ui->myTable->currentRow(),0)->data(1).toString();
            if(!code.isEmpty()){
                signalM::instance()->sendremoveMyGP(code);
            }
        }
    });


    m_myLeftMenu->addAction(removeMy);

}


void MainWindow::on_pushButton_clicked()
{
    signalM::instance()->sendposthttpGp(m_currentZQ+ui->searchEdit->text());

}


void MainWindow::on_comboBox_activated(const QString &arg1)
{
    if(arg1.contains("上")){
        m_currentZQ="sh";
    }
    else if(arg1.contains("深")){
        m_currentZQ="sz";
    }
}

void MainWindow::on_miniTable_doubleClicked(const QModelIndex &index)
{
    //    QMutexLocker locker(App->m_mutex);
    //    if(ui->miniTable->currentRow()>=0){
    //        QString code=ui->miniTable->item(ui->miniTable->currentRow(),0)->data(1).toString();
    //        if(!code.isEmpty()){
    //            removeGP(code);
    //        }
    //    }
}


void MainWindow::on_normalBtn_clicked()
{
    //    if(ui->normalFrame->isVisible()){
    //        ui->normalFrame->setVisible(false);
    //    }
    //    else {
    //        ui->normalFrame->setVisible(true);
    //    }
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_detailedBtn_clicked()
{
    if(ui->tableWidget->isVisible()){
        ui->tableWidget->setVisible(false);
    }
    else {
        ui->tableWidget->setVisible(true);
    }
}

void MainWindow::on_myBtn_clicked()
{
    //    if(ui->myTable->isVisible()){
    //        ui->myTable->setVisible(false);
    //    }
    //    else {
    //        ui->myTable->setVisible(true);
    //    }
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_addMyBtn_clicked()
{
    addMoneyWidget addDialog;
    addDialog.setCodecData(m_mGp);
    addDialog.exec();
}

void MainWindow::slotDataGPsChange(MapdataGP map)
{

    m_mGp=map.map;
    refreshNormalWidget();
    refreshLabel();
}

void MainWindow::slotDataHaveGPsChange(MapdataHaveGP map)
{
    m_mMyGp=map.map;
    refreshMyhaveWidget();
}

void MainWindow::slotDataAllDPChange(DataAllDP data)
{
    m_myAllDP=data;
}
void MainWindow::refreshNormalWidget()
{
    ui->tableWidget->setRowCount(m_mGp.count());
    ui->miniTable->setRowCount(m_mGp.count());
    if(m_mGp.count()>0){
        //        sendData();
        //        sendData2();

        int index=0;
        for(auto gp:m_mGp){
            ui->tableWidget->setItem(index,0,new QTableWidgetItem(gp.name));
            ui->tableWidget->setItem(index,1,new QTableWidgetItem(gp.codec));
            ui->tableWidget->setItem(index,2,new QTableWidgetItem(gp.TodayOpeningPrice));
            ui->tableWidget->setItem(index,3,new QTableWidgetItem(gp.YesterdayClosingPrice));
            ui->tableWidget->setItem(index,4,new QTableWidgetItem(gp.currentPrice));
            ui->tableWidget->setItem(index,5,new QTableWidgetItem(gp.todayMax));
            ui->tableWidget->setItem(index,6,new QTableWidgetItem(gp.todayMin));
            ui->tableWidget->setItem(index,7,new QTableWidgetItem(gp.Date));
            ui->tableWidget->setItem(index,8,new QTableWidgetItem(gp.Time));
            QString name1=gp.name;
            QString codec=gp.codec;
            QString cuurent=gp.currentPrice;
            double zd=gp.currentPrice.toDouble()-gp.YesterdayClosingPrice.toDouble();
            QString zdStr=QString::number(zd);
            double zf=zd*100.0/gp.YesterdayClosingPrice.toDouble();
            QString zfStr=QString::number(zf,'g',3)+"%";
            QTableWidgetItem *nameitem = new QTableWidgetItem(name1);
            nameitem->setData(1,gp.codec);
            QTableWidgetItem *currentItem = new QTableWidgetItem(cuurent);
            QTableWidgetItem *zdStrItem = new QTableWidgetItem(zdStr);
            QTableWidgetItem *zfStrItem = new QTableWidgetItem(zfStr);
            if(zf>0){
                nameitem->setTextColor(QColor("white"));
                nameitem->setBackground(QColor("red"));
                currentItem->setTextColor(QColor("white"));
                currentItem->setBackground(QColor("red"));
                zdStrItem->setTextColor(QColor("white"));
                zdStrItem->setBackground(QColor("red"));
                zfStrItem->setTextColor(QColor("white"));
                zfStrItem->setBackground(QColor("red"));
            }
            else{
                nameitem->setTextColor(QColor("white"));
                nameitem->setBackground(QColor("green"));
                currentItem->setTextColor(QColor("white"));
                currentItem->setBackground(QColor("green"));
                zdStrItem->setTextColor(QColor("white"));
                zdStrItem->setBackground(QColor("green"));
                zfStrItem->setTextColor(QColor("white"));
                zfStrItem->setBackground(QColor("green"));
            }
            ui->miniTable->setItem(index,0,nameitem);
            ui->miniTable->setItem(index,1,currentItem);
            ui->miniTable->setItem(index,2,zdStrItem);
            ui->miniTable->setItem(index,3,zfStrItem);

            index++;
        }
    }
}


void MainWindow::refreshMyhaveWidget()
{
    m_myAllDP=DataAllDP();
    ui->myTable->setRowCount(m_mMyGp.count()+1);
    int index1=0;
    DataHaveGP allgp;
    for(auto gp:m_mMyGp){
        allgp.haveNum+=gp.haveNum;
        allgp.todaySY+=gp.todaySY;
        allgp.historySY+=gp.historySY;
        allgp.payallPrice+=gp.payallPrice;
        allgp.currentPrice+=gp.currentPrice;
        allgp.yesterDayPrice+=gp.yesterDayPrice;
        allgp.currentallPrice+=gp.currentallPrice;
        ui->myTable->setItem(index1,0,new QTableWidgetItem(gp.name));
        ui->myTable->item(index1,0)->setData(1,gp.codec);;
        ui->myTable->setItem(index1,1,new QTableWidgetItem(QString::number(gp.haveNum)));
        ui->myTable->setItem(index1,2,new QTableWidgetItem(QString::number(gp.payallPrice)));
        ui->myTable->setItem(index1,3,new QTableWidgetItem(QString::number(gp.currentallPrice)));
        ui->myTable->setItem(index1,4,new QTableWidgetItem(QString::number(gp.currentPrice)));
        ui->myTable->setItem(index1,5,new QTableWidgetItem(QString::number(gp.historySY)));
        double historySyl=100*gp.historySY/gp.payallPrice;


        QString historyL=QString::number(historySyl,'g',3)+"%";
        ui->myTable->setItem(index1,6,new QTableWidgetItem(historyL));
        if(historySyl>0){
            ui->myTable->item(index1,5)->setTextColor(QColor("white"));
            ui->myTable->item(index1,6)->setTextColor(QColor("white"));
            ui->myTable->item(index1,5)->setBackground(QColor("red"));
            ui->myTable->item(index1,6)->setBackground(QColor("red"));
        }
        else {
            ui->myTable->item(index1,5)->setTextColor(QColor("white"));
            ui->myTable->item(index1,6)->setTextColor(QColor("white"));
            ui->myTable->item(index1,5)->setBackground(QColor("green"));
            ui->myTable->item(index1,6)->setBackground(QColor("green"));
        }
        ui->myTable->setItem(index1,7,new QTableWidgetItem(QString::number(gp.todaySY)));
        double todayyl=100*(gp.currentPrice-gp.yesterDayPrice)/gp.yesterDayPrice;
        QString todayL=QString::number(todayyl,'g',3)+"%";
        ui->myTable->setItem(index1,8,new QTableWidgetItem(todayL));
        if(todayyl>0){
            ui->myTable->item(index1,7)->setTextColor(QColor("white"));
            ui->myTable->item(index1,8)->setTextColor(QColor("white"));
            ui->myTable->item(index1,7)->setBackground(QColor("red"));
            ui->myTable->item(index1,8)->setBackground(QColor("red"));
        }
        else {
            ui->myTable->item(index1,7)->setTextColor(QColor("white"));
            ui->myTable->item(index1,8)->setTextColor(QColor("white"));
            ui->myTable->item(index1,7)->setBackground(QColor("green"));
            ui->myTable->item(index1,8)->setBackground(QColor("green"));
        }
        m_myAllDP.payallPrice+=gp.payallPrice;
        m_myAllDP.currentallPrice+=gp.currentallPrice;
        m_myAllDP.todaySY+=gp.todaySY;
        index1++;
    }
    ui->myTable->setItem(index1,0,new QTableWidgetItem("总收益"));
    ui->myTable->item(index1,0)->setData(1,"总收益");
    ui->myTable->setItem(index1,1,new QTableWidgetItem(QString::number(allgp.haveNum)));
    ui->myTable->setItem(index1,2,new QTableWidgetItem(QString::number(allgp.payallPrice)));
    ui->myTable->setItem(index1,3,new QTableWidgetItem(QString::number(allgp.currentallPrice)));
    ui->myTable->setItem(index1,4,new QTableWidgetItem(QString::number(allgp.currentPrice)));
    ui->myTable->setItem(index1,5,new QTableWidgetItem(QString::number(allgp.historySY)));
    double historySyl=100*allgp.historySY/allgp.payallPrice;


    QString historyL=QString::number(historySyl,'g',3)+"%";
    ui->myTable->setItem(index1,6,new QTableWidgetItem(historyL));
    if(historySyl>0){
        ui->myTable->item(index1,5)->setTextColor(QColor("white"));
        ui->myTable->item(index1,6)->setTextColor(QColor("white"));
        ui->myTable->item(index1,5)->setBackground(QColor("red"));
        ui->myTable->item(index1,6)->setBackground(QColor("red"));
    }
    else {
        ui->myTable->item(index1,5)->setTextColor(QColor("white"));
        ui->myTable->item(index1,6)->setTextColor(QColor("white"));
        ui->myTable->item(index1,5)->setBackground(QColor("green"));
        ui->myTable->item(index1,6)->setBackground(QColor("green"));
    }
    ui->myTable->setItem(index1,7,new QTableWidgetItem(QString::number(allgp.todaySY)));
    double todayyl=100*(allgp.todaySY)/allgp.currentallPrice;
    QString todayL=QString::number(todayyl,'g',3)+"%";
    ui->myTable->setItem(index1,8,new QTableWidgetItem(todayL));
    if(todayyl>0){
        ui->myTable->item(index1,7)->setTextColor(QColor("white"));
        ui->myTable->item(index1,8)->setTextColor(QColor("white"));
        ui->myTable->item(index1,7)->setBackground(QColor("red"));
        ui->myTable->item(index1,8)->setBackground(QColor("red"));
    }
    else {
        ui->myTable->item(index1,7)->setTextColor(QColor("white"));
        ui->myTable->item(index1,8)->setTextColor(QColor("white"));
        ui->myTable->item(index1,7)->setBackground(QColor("green"));
        ui->myTable->item(index1,8)->setBackground(QColor("green"));
    }
    cureentInfo="今日总收益:" +QString::number(allgp.todaySY) +" \n" +" 今日收益率"+ todayL;
    //    m_trayIcon->setToolTip(cureentInfo);
}

void MainWindow::refreshLabel()
{
    if(m_shLabel){
        DataGP gp=m_mGp.value("sh000001");

        double zd=gp.currentPrice.toDouble()-gp.YesterdayClosingPrice.toDouble();
        QString zdStr=QString::number(zd);
        double zf=zd*100.0/gp.YesterdayClosingPrice.toDouble();
        QString zfStr=QString::number(zf,'g',3)+"%";
        QString sh="上证指数";
        sh+="\n";
        sh+=gp.currentPrice;
        sh+="\n";
        sh+=zdStr;
        sh+="\n";
        sh+=zfStr;
        m_shLabel->setText(sh);
        if(zd>0){
            m_shLabel->setStyleSheet("QLabel { color:red; padding:2px; border:1px solid red; border-radius:15px; }");
        }
        else{
            m_shLabel->setStyleSheet("QLabel { color:green; padding:2px; border:1px solid green; border-radius:15px; }");
        }
    }
    if(m_szLabel){
        DataGP gp=m_mGp.value("sz399001");

        double zd=gp.currentPrice.toDouble()-gp.YesterdayClosingPrice.toDouble();
        QString zdStr=QString::number(zd);
        double zf=zd*100.0/gp.YesterdayClosingPrice.toDouble();
        QString zfStr=QString::number(zf,'g',3)+"%";
        QString sz="深圳成指";
        sz+="\n";
        sz+=gp.currentPrice;
        sz+="\n";
        sz+=zdStr;
        sz+="\n";
        sz+=zfStr;
        m_szLabel->setText(sz);
        if(zd>0){
            m_szLabel->setStyleSheet("QLabel { color:red; padding:2px; border:1px solid red; border-radius:15px; }");
        }
        else{
            m_szLabel->setStyleSheet("QLabel { color:green; padding:2px; border:1px solid green; border-radius:15px; }");
        }
    }
    if(m_cyLabel){
        DataGP gp=m_mGp.value("sz399006");

        double zd=gp.currentPrice.toDouble()-gp.YesterdayClosingPrice.toDouble();
        QString zdStr=QString::number(zd);
        double zf=zd*100.0/gp.YesterdayClosingPrice.toDouble();
        QString zfStr=QString::number(zf,'g',3)+"%";
        QString cy="创业板指";
        cy+="\n";
        cy+=gp.currentPrice;
        cy+="\n";
        cy+=zdStr;
        cy+="\n";
        cy+=zfStr;
        m_cyLabel->setText(cy);
        if(zd>0){
            m_cyLabel->setStyleSheet("QLabel { color:red; padding:2px; border:1px solid red; border-radius:15px; }");
        }
        else{
            m_cyLabel->setStyleSheet("QLabel { color:green; padding:2px; border:1px solid green; border-radius:15px; }");
        }
    }
};

void MainWindow::on_miniTable_customContextMenuRequested(const QPoint &pos)
{
    m_normalLeftMenu->exec(QCursor::pos());
}

void MainWindow::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    m_normalLeftMenu->exec(QCursor::pos());
}


void MainWindow::on_myTable_customContextMenuRequested(const QPoint &pos)
{
    m_myLeftMenu->exec(QCursor::pos());
}

void MainWindow::on_myTable_cellDoubleClicked(int row, int column)
{
    qDebug()<<row<<column;
    QTableWidgetItem *item=ui->myTable->item(row,0);
    QString key=item->data(1).toString();
    DataHaveGP haveGp=m_mMyGp.value(key);
    if(!haveGp.codec.isEmpty()){
        updateMyGpDialog dialog(haveGp);
        dialog.exec();
    }
    qDebug()<<11;
}

void MainWindow::on_miniTable_cellDoubleClicked(int row, int column)
{
    if(ui->miniTable->currentRow()>=0){
        QString code=ui->miniTable->item(ui->miniTable->currentRow(),0)->data(1).toString();
        if(!code.isEmpty()){
            QString codec=code.replace("sz","1");
            codec==codec.replace("sh","0");
            char*  chSecID;
            QByteArray baSecID = codec.toLatin1(); // must
            chSecID=baSecID.data();
            if(!m_stockWidget){
                m_stockWidget =new StockCanvas(codec);
                m_stockWidget->setMinimumSize(400,300);
            }
            else {
                m_stockWidget->setIDandTime(chSecID);
            }
            m_stockWidget->setWindowTitle(ui->miniTable->item(ui->miniTable->currentRow(),0)->text());
            m_stockWidget->show();
        }
    }

}

void MainWindow::on_tableWidget_cellDoubleClicked(int row, int column)
{
    if(ui->tableWidget->currentRow()>=0){
        QString code=ui->tableWidget->item(ui->tableWidget->currentRow(),1)->data(0).toString();
        if(!code.isEmpty()){
            QString codec=code.replace("sz","1");
            codec==codec.replace("sh","0");
            char*  chSecID;
            QByteArray baSecID = codec.toLatin1(); // must
            chSecID=baSecID.data();
            if(!m_stockWidget){
                m_stockWidget =new StockCanvas(codec);
                m_stockWidget->setMinimumSize(400,300);
            }
            else {
                m_stockWidget->setIDandTime(chSecID);
            }
            m_stockWidget->setWindowTitle(ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text());
            m_stockWidget->show();
        }
    }
}
