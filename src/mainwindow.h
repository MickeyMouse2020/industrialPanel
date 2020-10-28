#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "qcustomplot.h"
#include "api/app.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //����qcustomplot��ͼ���ԣ�ʵʱ
    void setupRealtimeDataDemo(QCustomPlot *customPlot);

protected:
    void closeEvent(QCloseEvent* event);

private slots:
    //���ʵʱ���ݲ�
    void realtimeDataSlot();

private:
    Ui::MainWindow *ui;
    //��ʱ�������ڵ���realtimeDataSlot()�ۣ�ʵ�ֶ�̬������ӵ�����
    QTimer dataTimer;

    void init();

    QVector<qreal> data1;
    QVector<qreal> data2;

    int data1Size;
    int data2Size;

    int i;
    int j;
};

#endif // MAINWINDOW_H