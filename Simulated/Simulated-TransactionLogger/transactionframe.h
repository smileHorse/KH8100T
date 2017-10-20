#ifndef TRANSACTIONFRAME_H
#define TRANSACTIONFRAME_H

#include "MasterServiceThread.h"
#include "SlaveServiceThread.h"

#include <QtWidgets/QMainWindow>

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
	void createWidgets();
	void createMenus();
	void createToolBars();
	void createStatusBar();
	void createActions();
	void createConnects();

	void updateStartStopAction(bool isStart);

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
	void outputOperationInfo(QString text);


private:
	QTextEdit*	textEdit;

	QAction* closeAction;			// 关闭程序
	QAction* startMasterAction;		// 启动主服务
	QAction* startSlaveAction;		// 启动备服务
	QAction* stopAction;			// 停止服务
	QAction* selectAction;			// 查询实时库数据
	QAction* clearAction;			// 清空文本

	QMenu*	fileMenu;
	QMenu*	operMenu;

	QToolBar*	fileToolbar;
	QToolBar*	operToolbar;

	MasterServiceThread	masterService;
	SlaveServiceThread slaveService;
};

#endif // TRANSACTIONFRAME_H
