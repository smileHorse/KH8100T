#include "transactionframe.h"
#include "RandomOperateDb.h"
#include "FastdbManager.h"
#include "fastdb.h"

#include <QtWidgets/QtWidgets>

#define Thread_Count	10

TransactionFrame::TransactionFrame(QWidget *parent)
	: QMainWindow(parent)
{
	m_fastdbManager = FastdbManagerInstance::getFastdbManagerInstance();

	createWidgets();
	createServices();
	createActions();
	createConnects();
	createMenus();
	createToolBars();
	createStatusBar();	
}

TransactionFrame::~TransactionFrame()
{
	Q_FOREACH(MasterServiceThread* thread, vctMasterServices)
	{
		if (thread)
		{
			delete thread;
			thread = NULL;
		}
	}

	if (slaveService)
	{
		delete slaveService;
		slaveService = NULL;
	}

	FastdbManagerInstance::releaseFastdbManagerInstance();
}

void TransactionFrame::createServices()
{
	openDatabase();

	vctMasterServices.reserve(Thread_Count);
	for (int i = 0; i < Thread_Count; ++i)
	{
		vctMasterServices.push_back(new MasterServiceThread());
	}
	slaveService = new SlaveServiceThread();
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
	operMenu->addSeparator();
	operMenu->addAction(restoreAction);
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
	operToolbar->addSeparator();
	operToolbar->addAction(restoreAction);
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

	restoreAction = new QAction(QIcon(":/images/restore.png"), QStringLiteral("恢复实时库"), this);
	restoreAction->setStatusTip(QStringLiteral("恢复实时库"));
	connect(restoreAction, SIGNAL(triggered()), this, SLOT(restoreDb()));
}

void TransactionFrame::createConnects()
{
	Q_FOREACH(MasterServiceThread* thread, vctMasterServices)
	{
		connect(this, SIGNAL(startMasterServiceSignal()), thread, SLOT(start()));
	}
	connect(this, SIGNAL(startSlaveServiceSignal()), slaveService, SLOT(start()));

	Q_FOREACH(MasterServiceThread* thread, vctMasterServices)
	{
		connect(thread, SIGNAL(outputOperationInfo(QString)), this, SLOT(outputOperationInfo(QString)));
	}
	connect(slaveService, SIGNAL(outputOperationInfo(QString)), this, SLOT(outputOperationInfo(QString)));

	connect(m_fastdbManager, SIGNAL(outputOperationInfo(QString)), this, SLOT(outputOperationInfo(QString)));
}

bool TransactionFrame::openDatabase()
{
	return m_fastdbManager->openDatabase();
}

bool TransactionFrame::reopenDatabase()
{
	return m_fastdbManager->reopenDatabase();
}

bool TransactionFrame::closeDatabase()
{
	return m_fastdbManager->closeDatabase();
}

void TransactionFrame::updateStartStopAction( bool isStart )
{
	startMasterAction->setEnabled(!isStart);
	startSlaveAction->setEnabled(!isStart);
	stopAction->setEnabled(isStart);
	selectAction->setEnabled(!isStart);
}

void TransactionFrame::setCurrentServiceType( ServiceType type )
{
	m_currServiceType = type;
}

ServiceType TransactionFrame::getCurrentServiceType() const
{
	return m_currServiceType;
}

QString TransactionFrame::getRestoreStatus( dbFileTransactionLogger::RestoreStatus status )
{
	switch(status)
	{
	case dbFileTransactionLogger::rsOK:
		return QStringLiteral("rsOK");
	case dbFileTransactionLogger::rsCRCMismatch:
		return QStringLiteral("rsCRCMismatch");
	case dbFileTransactionLogger::rsReadFailed:
		return QStringLiteral("rsReadFailed");
	case dbFileTransactionLogger::rsTableNotFound:
		return QStringLiteral("rsTableNotFound");
	case  dbFileTransactionLogger::rsOIDMismatch:
		return QStringLiteral("rsOIDMismatch");
	default:
		return QStringLiteral("Unknown");
	}
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
	openDatabase();
	
	outputOperationInfo(LoggerInfo::getLoggerInfo(QStringLiteral("打开实时库和事务日志文件成功"), FrameService));

	setCurrentServiceType(MasterService);

	// 启动事务日志管理线程
	if (dbTransactionLoggerThread.isRunning())
	{
		dbTransactionLoggerThread.setStop(true);

		dbTransactionLoggerThread.wait();
	}
	dbTransactionLoggerThread.start();

	emit startMasterServiceSignal();
}

void TransactionFrame::startSlaveService()
{
	updateStartStopAction(true);
	closeDatabase();

	setCurrentServiceType(SlaveService);

	emit startSlaveServiceSignal();
}

void TransactionFrame::stopService()
{
	updateStartStopAction(false);

	ServiceType currServiceType = getCurrentServiceType();
	if (currServiceType == MasterService)
	{
		Q_FOREACH(MasterServiceThread* thread, vctMasterServices)
		{
			if (thread->isRunning())
			{
				thread->setStop(true);
				thread->wait();
			}
		}
		
		dbTransactionLoggerThread.setStop(true);
		dbTransactionLoggerThread.wait();
	}
	else if (currServiceType == SlaveService)
	{
		if (slaveService->isRunning())
		{
			slaveService->setStop(true);
			slaveService->wait();
		}
	}
	outputOperationInfo(LoggerInfo::getLoggerInfo(QStringLiteral("关闭实时库和事务日志文件成功"), FrameService));

	if (currServiceType == MasterService)
	{
		closeDatabase();
	}
}

void TransactionFrame::selectData()
{
	openDatabase();

	RandomOperateDb operDb;
	QString threadId = QString("%1").arg((DWORD)(QThread::currentThreadId()));
	operDb.setThreadId(threadId);
	operDb.startOperate(SelectMode);

	outputOperationInfo(LoggerInfo::getLoggerInfo(operDb.getOperateInfo(), FrameService));

}

void TransactionFrame::clearTextEdit()
{
	if (textEdit)
	{
		textEdit->clear();
	}
}

void TransactionFrame::restoreDb()
{
	// 通过事务日志恢复实时库时，必须关闭实时库，并重新打开，才能进行后续恢复操作
	reopenDatabase();

	QString fileName = QFileDialog::getOpenFileName(this, QStringLiteral("选择备份文件"));
	if (fileName.isEmpty())
	{
		return;
	}
	string path = fileName.toStdString();
	if (!m_fastdbManager->openTransactionLogger(path, dbFile::read_only))
	{
		outputOperationInfo(LoggerInfo::getLoggerInfo(QStringLiteral("打开事务日志文件失败"), FrameService));

		return;
	}

	outputOperationInfo(LoggerInfo::getLoggerInfo(QStringLiteral("打开事务日志文件成功"), FrameService));
	
	try 
	{
		dbFileTransactionLogger* tlPtr = m_fastdbManager->getDbFileTransactionLogger();
		size_t n;
		dbFileTransactionLogger::RestoreStatus status = tlPtr->restore(*(m_fastdbManager->getDbDatabase()), n);
		if (status == dbFileTransactionLogger::rsOK)
		{
			tlPtr->close();
			outputOperationInfo(LoggerInfo::getLoggerInfo(QStringLiteral("恢复实时库成功"), FrameService));
		}
		else
		{
			//tlPtr->rollback();
			QString text = QStringLiteral("恢复实时库失败\t错误原因: %1").arg(getRestoreStatus(status));
			outputOperationInfo(LoggerInfo::getLoggerInfo(text, FrameService));
		}
	}
	catch(const dbException& ex)
	{
		QString text = QStringLiteral("恢复实时库异常\t异常原因: %1").arg(QString().fromStdString(ex.what()));
		outputOperationInfo(LoggerInfo::getLoggerInfo(text, FrameService));
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

