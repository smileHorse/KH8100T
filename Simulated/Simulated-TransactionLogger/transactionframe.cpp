#include "transactionframe.h"
#include "RandomOperateDb.h"
#include "common.h"
#include "fastdb.h"

#include <QtWidgets/QtWidgets>

TransactionFrame::TransactionFrame(QWidget *parent)
	: QMainWindow(parent)
{
	createWidgets();
	createActions();
	createConnects();
	createMenus();
	createToolBars();
	createStatusBar();
}

TransactionFrame::~TransactionFrame()
{

}

void TransactionFrame::createWidgets()
{
	textEdit = new QTextEdit();
	setCentralWidget(textEdit);

	resize(800, 600);
	setWindowTitle(QStringLiteral("Fastdb备份恢复"));
	setWindowIcon(QIcon(":/images/logger.png"));
}

void TransactionFrame::createMenus()
{
	fileMenu = menuBar()->addMenu(QIcon(":/images/file.png"), QStringLiteral("文件"));
	fileMenu->addAction(closeAction);

	operMenu = menuBar()->addMenu(QIcon(":/images/oper.png"), QStringLiteral("操作"));
	operMenu->addAction(startMasterAction);
	operMenu->addAction(startSlaveAction);
	operMenu->addAction(stopAction);
	operMenu->addSeparator();
	operMenu->addAction(selectAction);
	operMenu->addAction(clearAction);
}

void TransactionFrame::createToolBars()
{
	fileToolbar = addToolBar(QStringLiteral("文件"));
	fileToolbar->addAction(closeAction);

	operToolbar = addToolBar(QStringLiteral("操作"));
	operToolbar->addAction(startMasterAction);
	operToolbar->addAction(startSlaveAction);
	operToolbar->addAction(stopAction);
	operToolbar->addSeparator();
	operToolbar->addAction(selectAction);
	operToolbar->addAction(clearAction);
}

void TransactionFrame::createStatusBar()
{
	statusBar()->showMessage(QStringLiteral("准备就绪"));
}

void TransactionFrame::createActions()
{
	closeAction = new QAction(QIcon(":/images/close.png"), QStringLiteral("关闭"), this);
	closeAction->setStatusTip(QStringLiteral("退出程序"));
	connect(closeAction, SIGNAL(triggered()), this, SLOT(closeApp()));

	startMasterAction = new QAction(QIcon(":/images/startMaster.png"), QStringLiteral("启动主服务"), this);
	startMasterAction->setStatusTip(QStringLiteral("启动主服务"));
	connect(startMasterAction, SIGNAL(triggered()), this, SLOT(startMasterService()));

	startSlaveAction = new QAction(QIcon(":/images/startSlave.png"), QStringLiteral("启动备服务"), this);
	startSlaveAction->setStatusTip(QStringLiteral("启动备服务"));
	connect(startSlaveAction, SIGNAL(triggered()), this, SLOT(startSlaveService()));

	stopAction = new QAction(QIcon(":/images/stop.png"), QStringLiteral("停止服务"), this);
	stopAction->setStatusTip(QStringLiteral("停止服务"));
	stopAction->setEnabled(false);
	connect(stopAction, SIGNAL(triggered()), this, SLOT(stopService()));

	selectAction = new QAction(QIcon(":/images/select.png"), QStringLiteral("查询数据"), this);
	selectAction->setStatusTip(QStringLiteral("查询数据"));
	connect(selectAction, SIGNAL(triggered()), this, SLOT(selectData()));

	clearAction = new QAction(QIcon(":/images/clear.png"), QStringLiteral("清空文本"), this);
	clearAction->setStatusTip(QStringLiteral("清空文本"));
	connect(clearAction, SIGNAL(triggered()), this, SLOT(clearTextEdit()));
}

void TransactionFrame::createConnects()
{
	connect(this, SIGNAL(startMasterServiceSignal()), &masterService, SLOT(start()));
	connect(this, SIGNAL(startSlaveServiceSignal()), &slaveService, SLOT(start()));

	connect(&masterService, SIGNAL(outputOperationInfo(QString)), this, SLOT(outputOperationInfo(QString)));
	connect(&slaveService, SIGNAL(outputOperationInfo(QString)), this, SLOT(outputOperationInfo(QString)));
}

void TransactionFrame::updateStartStopAction( bool isStart )
{
	startMasterAction->setEnabled(!isStart);
	startSlaveAction->setEnabled(!isStart);
	stopAction->setEnabled(isStart);
	selectAction->setEnabled(!isStart);
}

void TransactionFrame::closeApp()
{
	if(QMessageBox::information(this, QStringLiteral("退出"), QStringLiteral("是否退出当前应用程序"), 
		QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok)
	{
		stopService();
		close();
	}
	return;
}

void TransactionFrame::startMasterService()
{
	updateStartStopAction(true);

	emit startMasterServiceSignal();
}

void TransactionFrame::startSlaveService()
{
	updateStartStopAction(true);

	emit startSlaveServiceSignal();
}

void TransactionFrame::stopService()
{
	updateStartStopAction(false);

	if (masterService.isRunning())
	{
		masterService.setStop(true);
	}
	if (slaveService.isRunning())
	{
		slaveService.setStop(true);
	}
}

void TransactionFrame::selectData()
{
	dbDatabase db(dbDatabase::dbReadOnly);
	if (!db.open(DatabaseName))
	{
		outputOperationInfo(LoggerInfo::getLoggerInfo(QStringLiteral("打开实时库失败"), FrameService));
		return;
	}

	RandomOperateDb operDb(&db);
	operDb.startOperate(SelectMode);

	outputOperationInfo(LoggerInfo::getLoggerInfo(operDb.getOperateInfo(), FrameService));

	db.close();
}

void TransactionFrame::clearTextEdit()
{
	if (textEdit)
	{
		textEdit->clear();
	}
}

void TransactionFrame::outputOperationInfo( QString text )
{
	if (textEdit)
	{
		textEdit->moveCursor(QTextCursor::End);
		textEdit->insertPlainText(text);
		textEdit->moveCursor(QTextCursor::End);
	}
}

