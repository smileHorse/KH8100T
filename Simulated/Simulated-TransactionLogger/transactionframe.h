#ifndef TRANSACTIONFRAME_H
#define TRANSACTIONFRAME_H

#include "MasterServiceThread.h"
#include "SlaveServiceThread.h"
#include "DbTransactionLoggerThread.h"
#include "fastdb.h"
#include "common.h"

#include <QtCore/QtCore>
#include <QtWidgets/QMainWindow>

class FastdbManager;

class QTextEdit;
class QAction;
class QMenu;
class QToolBar;

class TransactionFrame : public QMainWindow
{
	Q_OBJECT

public:
	TransactionFrame(QWidget *parent = 0);
	~TransactionFrame();

private:
	void createServices();
	void createWidgets();
	void createMenus();
	void createToolBars();
	void createStatusBar();
	void createActions();
	void createConnects();

	bool openDatabase();
	bool reopenDatabase();
	bool closeDatabase();

	void updateStartStopAction(bool isStart);

	void setCurrentServiceType(ServiceType type);
	ServiceType getCurrentServiceType() const;

	QString getRestoreStatus(dbFileTransactionLogger::RestoreStatus status);

signals:
	void startMasterServiceSignal();
	void startSlaveServiceSignal();

private slots:
	void closeApp();
	void startMasterService();
	void startSlaveService();
	void stopService();
	void selectData();
	void clearTextEdit();
	void restoreDb();
	void outputOperationInfo(QString text);


private:
	QTextEdit*	textEdit;

	QAction* closeAction;			// 关闭程序
	QAction* startMasterAction;		// 启动主服务
	QAction* startSlaveAction;		// 启动备服务
	QAction* stopAction;			// 停止服务
	QAction* selectAction;			// 查询实时库数据
	QAction* clearAction;			// 清空文本
	QAction* restoreAction;			// 恢复实时库

	QMenu*	fileMenu;
	QMenu*	operMenu;

	QToolBar*	fileToolbar;
	QToolBar*	operToolbar;

	QVector<MasterServiceThread*>	vctMasterServices;
	SlaveServiceThread* slaveService;
	DbTransactionLoggerThread	dbTransactionLoggerThread;

	FastdbManager*	m_fastdbManager;

	ServiceType	m_currServiceType;
};

#endif // TRANSACTIONFRAME_H
