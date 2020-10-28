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
    //设置qcustomplot画图属性，实时
    void setupRealtimeDataDemo(QCustomPlot *customPlot);

protected:
    void closeEvent(QCloseEvent* event);

private slots:
    //添加实时数据槽
    void realtimeDataSlot();

private:
    Ui::MainWindow *ui;
    //定时器，周期调用realtimeDataSlot()槽，实现动态数据添加到曲线
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