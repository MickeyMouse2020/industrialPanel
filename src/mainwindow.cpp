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

    // ΪQCustomePlot���ñ���ɫ
    ui->customPlot->setBackground(pixmap);
    ui->customPlot->replot();

    setupRealtimeDataDemo(ui->customPlot);
    ui->customPlot->replot();

    ui->checkBox_temp->setChecked(true);
    ui->checkBox_hui->setChecked(true);

    // �ö���ǰ����
    ::SetWindowPos(HWND(this->winId()), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

    // ��dat�ļ��ָ����ڴ�Сλ��
    restoreGeometry(App::loadDataHelper(qApp->applicationDirPath() + "/../data/MainWindow"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    // ���洰�ڴ�Сλ�õ�dat�ļ�
    App::storeDataHelper(qApp->applicationDirPath() + "/../data/MainWindow", saveGeometry());
}

//��ͼ��ʼ��
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

  // ����ͼ���ϵ������С
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
    // key�ĵ�λ����
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
        ui->customPlot->graph(0)->addData(key, value0);//�������1������1
    if (ui->checkBox_hui->isChecked())
        ui->customPlot->graph(1)->addData(key, value1);//�������2������2

    //ɾ��8��֮ǰ�����ݡ������8Ҫ���������ú������ȵ�8�������
    //��������Ҫ��Ч�������Ե���������ֵ���۲���ʾ��Ч����
    /*ui->customPlot->graph(0)->removeDataBefore(key-8);
    ui->customPlot->graph(1)->removeDataBefore(key-8);*/
    ui->customPlot->graph(0)->removeDataBefore(key - 30);
    ui->customPlot->graph(1)->removeDataBefore(key - 30);

    //�Զ��趨graph(1)����y��ķ�Χ��������趨���п��ܿ�����ͼ��
    //Ҳ������ui->customPlot->yAxis->setRange(up,low)�ֶ��趨y�᷶Χ
    ui->customPlot->graph(0)->rescaleValueAxis();
    ui->customPlot->graph(1)->rescaleValueAxis(true);   

    //�����8����ָ������ʱ����Ϊ8�룬�����Ҫ��������ʾ�����ʱ��
    //�Ͱ�8����Ϊ�Ƚϴ�ֵ������Ҫ��ʾ60�룬�Ǿ͸ĳ�60��
    //��ʱremoveDataBefore(key-8)�е�8ҲҪ�ĳ�60������������ʾ��������
    //ui->customPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);//�趨x��ķ�Χ
    ui->customPlot->xAxis->setRange(key+0.25, 30, Qt::AlignRight);//�趨x��ķ�Χ
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
