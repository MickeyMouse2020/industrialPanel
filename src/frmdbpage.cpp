#pragma execution_character_set("utf-8")

#include "frmdbpage.h"
#include "ui_frmdbpage.h"
#include "dbpage.h"

frmDbPage::frmDbPage(QWidget *parent) : QWidget(parent), ui(new Ui::frmDbPage)
{
    ui->setupUi(this);
    this->initForm();

    on_btnSelect_clicked();
}

frmDbPage::~frmDbPage()
{
    delete ui;
}

void frmDbPage::initForm()
{
    columnNames.clear();
    columnWidths.clear();

    tableName = App::dbTableName;
    countName = "id";

    columnNames.append("序号");
    columnNames.append("属性名");
    columnNames.append("单位");
    columnNames.append("实测数据");
    columnNames.append("模拟数据");
    columnNames.append("测量时间");
    columnNames.append("其他");

    columnWidths.append(75);
    columnWidths.append(150);
    columnWidths.append(120);
    columnWidths.append(150);
    columnWidths.append(150);
    columnWidths.append(480);
    columnWidths.append(100);

    //设置需要显示数据的表格和翻页的按钮
    dbPage = new DbPage(this);
    //设置所有列居中显示
    dbPage->setAllCenter(true);
    dbPage->setControl(ui->tableMain, ui->labPageCount, ui->labPageCurrent, ui->labResultCount, ui->labResultCurrent, ui->labResult, 0,
                       ui->btnFirst, ui->btnPre, ui->btnNext, ui->btnLast, countName);
    ui->tableMain->verticalHeader()->setDefaultSectionSize(25);
    ui->tableMain->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void frmDbPage::on_btnSelect_clicked()
{
    ui->labResult->setText("正在查询...");

    //绑定数据到表格
    QString sql = "where 1=1";
    dbPage->setTableName(tableName);
    dbPage->setOrderSql(QString("%1 %2").arg(countName).arg("asc"));
    dbPage->setWhereSql(sql);
    //dbPage->setResultCurrent(20);
    dbPage->setResultCurrent(15);
    dbPage->setColumnNames(columnNames);
    dbPage->setColumnWidths(columnWidths);
    dbPage->select();
}
