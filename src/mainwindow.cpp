#pragma execution_character_set("utf-8")

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVector>
#include <QTimer>
#include <QTime>
#include <QSqlQuery>
#include <QFileInfo>

#define LineWidth 3

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    i(0), j(0)
{
    init();

    ui->setupUi(this);

    this->setWindowTitle(App::windowTitle);

    QPixmap pixmap(":/vpe/img/bg1.jpg");

    // 为QCustomePlot设置背景色
    ui->customPlot->setBackground(pixmap);
    ui->customPlot->replot();

    setupRealtimeDataDemo(ui->customPlot);
    ui->customPlot->replot();

    ui->checkBox_temp->setChecked(true);
    ui->checkBox_hui->setChecked(true);

    // 置顶当前窗口
    ::SetWindowPos(HWND(this->winId()), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

    // 从dat文件恢复窗口大小位置
    restoreGeometry(App::loadDataHelper(qApp->applicationDirPath() + "/../data/MainWindow"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    // 保存窗口大小位置到dat文件
    App::storeDataHelper(qApp->applicationDirPath() + "/../data/MainWindow", saveGeometry());
}

//画图初始化
void MainWindow::setupRealtimeDataDemo(QCustomPlot *customPlot)
{
  customPlot->addGraph(); // blue line
  customPlot->graph(0)->setPen(QPen(Qt::blue, LineWidth));
  customPlot->graph(0)->setName(ui->checkBox_temp->text());
  customPlot->addGraph(); // red line
  customPlot->graph(1)->setPen(QPen(Qt::red, LineWidth));
  customPlot->graph(1)->setName(ui->checkBox_hui->text());

  customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
  customPlot->xAxis->setAutoTickStep(false);
  customPlot->xAxis->setTickStep(2);
  customPlot->axisRect()->setupFullAxesBox();

  // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
  //dataTimer.start(0); // Interval 0 means to refresh as fast as possible
  dataTimer.start(400);
  customPlot->legend->setVisible(true);

  customPlot->xAxis->setLabel(App::xAxisLabel);
  customPlot->yAxis->setLabel(App::yAxisLabel);

  //customPlot->yAxis->setNumberFormat("gbc");
  customPlot->yAxis->setNumberPrecision(4);

  customPlot->xAxis->setTickLabelRotation(-50);

  // 设置图表上的字体大小
  QFont font;
  font.setBold(true);
  font.setPointSize(12);

  customPlot->xAxis->setLabelFont(font);
  customPlot->yAxis->setLabelFont(font);

  font.setPointSize(10);
  customPlot->xAxis->setTickLabelFont(font);
  customPlot->yAxis->setTickLabelFont(font);
}

void MainWindow::realtimeDataSlot()
{
    // key的单位是秒
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    
    double value0 = 0;
    double value1 = 0;

    if (i >= data1Size) i = 0;
    if (j >= data2Size) j = 0;
    if (data1Size > 0 && data2Size > 0)
    {
        value0 = data1.at(i);
        value1 = data2.at(j);
    }
   
    i++;
    j++;

    if (ui->checkBox_temp->isChecked())
        ui->customPlot->graph(0)->addData(key, value0);//添加数据1到曲线1
    if (ui->checkBox_hui->isChecked())
        ui->customPlot->graph(1)->addData(key, value1);//添加数据2到曲线2

    //删除8秒之前的数据。这里的8要和下面设置横坐标宽度的8配合起来
    //才能起到想要的效果，可以调整这两个值，观察显示的效果。
    /*ui->customPlot->graph(0)->removeDataBefore(key-8);
    ui->customPlot->graph(1)->removeDataBefore(key-8);*/
    ui->customPlot->graph(0)->removeDataBefore(key - 30);
    ui->customPlot->graph(1)->removeDataBefore(key - 30);

    //自动设定graph(1)曲线y轴的范围，如果不设定，有可能看不到图像
    //也可以用ui->customPlot->yAxis->setRange(up,low)手动设定y轴范围
    ui->customPlot->graph(0)->rescaleValueAxis();
    ui->customPlot->graph(1)->rescaleValueAxis(true);   

    //这里的8，是指横坐标时间宽度为8秒，如果想要横坐标显示更多的时间
    //就把8调整为比较大到值，比如要显示60秒，那就改成60。
    //这时removeDataBefore(key-8)中的8也要改成60，否则曲线显示不完整。
    //ui->customPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);//设定x轴的范围
    ui->customPlot->xAxis->setRange(key+0.25, 30, Qt::AlignRight);//设定x轴的范围
    ui->customPlot->replot();
}

void MainWindow::init()
{
    QSqlQuery query("select actualData, simulationData from " + App::dbTableName);
    while (query.next())
    {
        data1.push_back(query.value(0).toReal());
        data2.push_back(query.value(1).toReal());
    }

    data1Size = data1.size();
    data2Size = data2.size();
}
