#pragma execution_character_set("utf-8")

#include "app.h"

QString App::windowTitle = "旋风压差面板";
int App::fontSize = 12;
QString App::xAxisLabel = "Time";
QString App::yAxisLabel = "旋风压差 / kPa";
QString App::dbTableName = "pressure";
QString App::ConfigFile = "config.ini";

bool App::fileIsExist(const QString& strFile)
{
    QFile tempFile(strFile);
    return tempFile.exists();
}

void App::readConfig()
{
    if (!checkConfig(App::ConfigFile)) {
        return;
    }

    QSettings set(App::ConfigFile, QSettings::IniFormat);
    set.setIniCodec(QTextCodec::codecForName("UTF-8"));//在此添加编码格式设置（如果INI文件编码为UTF-8则这里设置UTF-8，如果为GB2312，则这里设置GB2312，两者一致），即可读写ini文件中的中文

    set.beginGroup("Config");
    App::windowTitle = set.value("windowTitle").toString();
    App::fontSize = set.value("fontSize").toInt();
    App::xAxisLabel = set.value("xAxisLabel").toString();
    App::yAxisLabel = set.value("yAxisLabel").toString();
    App::dbTableName = set.value("dbTableName").toString();
    set.endGroup();
}

void App::writeConfig()
{
    QSettings set(App::ConfigFile, QSettings::IniFormat);
    set.setIniCodec(QTextCodec::codecForName("UTF-8"));//在此添加编码格式设置（如果INI文件编码为UTF-8则这里设置UTF-8，如果为GB2312，则这里设置GB2312，两者一致），即可读写ini文件中的中文

    set.beginGroup("Config");
    set.setValue("windowTitle", App::windowTitle);
    set.setValue("fontSize", App::fontSize);
    set.setValue("xAxisLabel", App::xAxisLabel);
    set.setValue("yAxisLabel", App::yAxisLabel);
    set.setValue("dbTableName", App::dbTableName);
    set.endGroup();
}

void App::newConfig()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
#endif
    writeConfig();
}

bool App::checkConfig(QString configFile)
{
    //如果配置文件大小为0,则以初始值继续运行,并生成配置文件
    QFile file(configFile);
    if (file.size() == 0) {
        newConfig();
        return false;
    }

    //如果配置文件不完整,则以初始值继续运行,并生成配置文件
    if (file.open(QFile::ReadOnly)) {
        bool ok = true;
        while (!file.atEnd()) {
            QString line = file.readLine();
            line = line.replace("\r", "");
            line = line.replace("\n", "");
            QStringList list = line.split("=");

            if (list.count() == 2) {
                if (list.at(1) == "") {
                    ok = false;
                    break;
                }
            }
        }

        if (!ok) {
            newConfig();
            return false;
        }
    }
    else {
        newConfig();
        return false;
    }

    return true;
}


QByteArray App::loadDataHelper(const QString& fname)
{
    QFile f(fname + QString(".dat"));
    if (f.open(QFile::ReadOnly))
    {
        QByteArray ba = f.readAll();
        f.close();
        return ba;
    }
    return QByteArray();
}

void App::storeDataHelper(const QString& fname, const QByteArray& ba)
{
    QFile f(fname + QString(".dat"));
    if (f.open(QFile::WriteOnly))
    {
        f.write(ba);
        f.close();
    }
}