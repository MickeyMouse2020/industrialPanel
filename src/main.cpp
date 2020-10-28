#pragma execution_character_set("utf-8")

#include "mainwindow.h"

#include <QApplication>
#include <QMutex>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFileInfo>

void outputMessage(QtMsgType type, const QMessageLogContext& context, const QString& msg);
bool createConnection();

//�ļ��Ƿ����
bool FileIsExist(QString strFile);

int main(int argc, char* argv[])
{
#ifdef QT_NO_DEBUG 
	//ע��MessageHandler
	qInstallMessageHandler(outputMessage); //ע��MessageHandler
#else
	//Debug��ֱ�������console
#endif

	QApplication app(argc, argv);

	App::ConfigFile = QString("%1/../%2/%3.ini").arg(qApp->applicationDirPath()).arg("data").arg("config");

	App::readConfig();
	app.setFont(QFont("Microsoft Yahei", App::fontSize));

	//�ж����ݿ��ļ��Ƿ����
	if (!FileIsExist(QCoreApplication::applicationDirPath() + "/../data/database.db")) {
		createConnection();
		qDebug() << "database.db������!";
	}
	else
	{
		qDebug() << "database.db����!";
		QSqlDatabase dbConn = QSqlDatabase::addDatabase("QSQLITE");
		dbConn.setDatabaseName(QCoreApplication::applicationDirPath() + "/../data/database.db");
		if (dbConn.open()) {
			qDebug() << "�������ݿ�ɹ�!";
		}
		else {
			qDebug() << "�������ݿ�ʧ��!";

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
	// ����
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

	// ���������Ϣ��ʽ
	QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
	QString strMessage = QString("Message:%1 \tFile:%2  Line:%3  Function:%4  DateTime:%5")
		.arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function).arg(strDateTime);

	// �����Ϣ���ļ��У���д��׷����ʽ��
	//QFile file("log.txt");
	//QFile file(Utils::AppPath() + "log.txt");
	QFile file(QCoreApplication::applicationDirPath() + "log.txt");
	file.open(QIODevice::ReadWrite | QIODevice::Append);
	QTextStream stream(&file);
	stream << strMessage << "\r\n";
	file.flush();
	file.close();

	// ����
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