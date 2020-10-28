#pragma execution_character_set("utf-8")

#include "mainwindow.h"

#include <QApplication>
#include <QMutex>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFileInfo>

void outputMessage(QtMsgType type, const QMessageLogContext& context, const QString& msg);
bool createConnection();

//文件是否存在
bool FileIsExist(QString strFile);

int main(int argc, char* argv[])
{
#ifdef QT_NO_DEBUG 
	//注册MessageHandler
	qInstallMessageHandler(outputMessage); //注册MessageHandler
#else
	//Debug版直接输出到console
#endif

	QApplication app(argc, argv);

	App::ConfigFile = QString("%1/../%2/%3.ini").arg(qApp->applicationDirPath()).arg("data").arg("config");

	App::readConfig();
	app.setFont(QFont("Microsoft Yahei", App::fontSize));

	//判断数据库文件是否存在
	if (!FileIsExist(QCoreApplication::applicationDirPath() + "/../data/database.db")) {
		createConnection();
		qDebug() << "database.db不存在!";
	}
	else
	{
		qDebug() << "database.db存在!";
		QSqlDatabase dbConn = QSqlDatabase::addDatabase("QSQLITE");
		dbConn.setDatabaseName(QCoreApplication::applicationDirPath() + "/../data/database.db");
		if (dbConn.open()) {
			qDebug() << "连接数据库成功!";
		}
		else {
			qDebug() << "连接数据库失败!";

		}
	}

	QString qssFile = ":/lightblue.css";

	QFile file(qssFile);

	if (file.open(QFile::ReadOnly)) {
		QString qss = QLatin1String(file.readAll());
		QString paletteColor = qss.mid(20, 7);
		app.setPalette(QPalette(QColor(paletteColor)));
		app.setStyleSheet(qss);
		file.close();
	}

	MainWindow mainWin;
	mainWin.show();

	return app.exec();
}

void outputMessage(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
	// 加锁
	static QMutex mutex;
	mutex.lock();

	QByteArray localMsg = msg.toLocal8Bit();

	QString strMsg("");
	switch (type)
	{
	case QtDebugMsg:
		strMsg = QString("Debug:");
		break;
	case QtWarningMsg:
		strMsg = QString("Warning:");
		break;
	case QtCriticalMsg:
		strMsg = QString("Critical:");
		break;
	case QtFatalMsg:
		strMsg = QString("Fatal:");
		break;
	}

	// 设置输出信息格式
	QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
	QString strMessage = QString("Message:%1 \tFile:%2  Line:%3  Function:%4  DateTime:%5")
		.arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function).arg(strDateTime);

	// 输出信息至文件中（读写、追加形式）
	//QFile file("log.txt");
	//QFile file(Utils::AppPath() + "log.txt");
	QFile file(QCoreApplication::applicationDirPath() + "log.txt");
	file.open(QIODevice::ReadWrite | QIODevice::Append);
	QTextStream stream(&file);
	stream << strMessage << "\r\n";
	file.flush();
	file.close();

	// 解锁
	mutex.unlock();
}

bool createConnection()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(QCoreApplication::applicationDirPath() + "/../data/database.db");
	if (!db.open()) return false;
	QSqlQuery query;

	bool createTableResult = query.exec(QString(
		"create table pressure ("
			"id INTEGER PRIMARY KEY AUTOINCREMENT, "
			"propertyName VARCHAR(40), "
			"propertyUnit VARCHAR(40), "
			"actualData INTEGER , "
			"simulationData INTEGER , "
			"triggerTime DATETIME, "
			"other VARCHAR(40))"));

	qDebug() << "createTableResult = " << createTableResult;

	return true;
}

bool FileIsExist(QString strFile) {
	QFile tempFile(strFile);
	return tempFile.exists();
}