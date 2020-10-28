#ifndef APP_H
#define APP_H

#include "head.h"

class App
{
public:
    //文件是否存在
    static bool fileIsExist(const QString& strFile);

    static QString xAxisLabel;
    static QString yAxisLabel;
    static QString dbTableName;

    //读写配置参数及其他操作
    static void readConfig();           //读取配置参数
    static void writeConfig();          //写入配置参数
    static void newConfig();            //以初始值新建配置文件

    static bool checkConfig(QString configFile);

    static QString ConfigFile;

    static QString windowTitle; // 主窗口标题
    static int fontSize;

    static QByteArray loadDataHelper(const QString& fname);
    static void storeDataHelper(const QString& fname, const QByteArray& ba);
};

#endif // APP_H
