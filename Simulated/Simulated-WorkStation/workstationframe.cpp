#include "workstationframe.h"
#include "TableHeader.h"
#include "workstationserver.h"
#include "workstationserverthread.h"
#include "OperationInfo.h"

#include <QtWidgets/QtWidgets>

WorkStationFrame::WorkStationFrame(int argc, char* argv[], QWidget *parent)
	: QMainWindow(parent), m_argc(argc), m_argv(argv)
{
	m_workStationServerPtr = new WorkStationServer(m_argc, m_argv);
	m_workStationServerThreadPtr = new WorkStationServerThread;

	createWidgets();

	createActions();
	createMenus();
	createToolBars();
	createStatusBar();
	createConnects();

}

WorkStationFrame::~WorkStationFrame()
{

}

void WorkStationFrame::createWidgets()
{
	tableWidget = new QTableWidget;
	QStringList tableHeaders = TableHeader::getTableHeaderLabels();
	tableWidget->setColumnCount(tableHeaders.count());
	tableWidget->setShowGrid(true);
	tableWidget->setHorizontalHeaderLabels(tableHeaders);

	textEdit = new QTextEdit;
	textEdit->setTextBackgroundColor(QColor(255, 0, 0));
	textEdit->setReadOnly(true);

	QSplitter* splitter = new QSplitter(Qt::Vertical);
	splitter->addWidget(tableWidget);
	splitter->addWidget(textEdit);
	splitter->setStretchFactor(0, 1);
	splitter->setStretchFactor(1, 4);
	setCentralWidget(splitter);

	OperationInfo info(TYPE_CLIENT, "操作信息测试");
	updateTableWidget(info);

	resize(1200, 800);
	setWindowTitle(QStringLiteral("工作站模拟机"));
	setWindowIcon(QIcon(":/images/workstation.png"));
}

void WorkStationFrame::createActions()
{
	startServerAction = new QAction(QIcon(":/images/startServer.png"), QStringLiteral("启动服务"), this);
	startServerAction->setStatusTip(QStringLiteral("启动Ice服务"));
	connect(startServerAction, SIGNAL(triggered()), this, SLOT(startServer()));

	stopServerAction = new QAction(QIcon(":/images/stopServer.png"), QStringLiteral("停止服务"), this);
	stopServerAction->setStatusTip(QStringLiteral("停止Ice服务"));
	stopServerAction->setEnabled(false);
	connect(stopServerAction, SIGNAL(triggered()), this, SLOT(stopServer()));

	exitAction = new QAction(QIcon(":/images/exit.png"), QStringLiteral("退出"), this);
	exitAction->setShortcut(QKeySequence::Quit);
	exitAction->setStatusTip(QStringLiteral("退出模拟机程序"));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
	
	requestStormRdbDataAction = new QAction(QIcon(":/images/requestStormRdb.png"), QStringLiteral("请求实时数据(IceStorm)"), this);
	requestStormRdbDataAction->setStatusTip(QStringLiteral("通过IceStorm请求实时数据"));
	requestStormRdbDataAction->setEnabled(false);
	connect(requestStormRdbDataAction, SIGNAL(triggered()), this, SLOT(requestStormRdbData()));
	
	requestStormTopoDataAction = new QAction(QIcon(":/images/requestStormTopo.png"), QStringLiteral("请求拓扑数据(IceStorm)"), this);
	requestStormTopoDataAction->setStatusTip(QStringLiteral("通过IceStorm请求拓扑数据"));
	requestStormTopoDataAction->setEnabled(false);
	connect(requestStormTopoDataAction, SIGNAL(triggered()), this, SLOT(requestStormTopoData()));

	requestRdbDataAction = new QAction(QIcon(":/images/requestRdb.png"), QStringLiteral("请求实时数据(Ice)"), this);
	requestRdbDataAction->setStatusTip(QStringLiteral("通过Ice请求实时数据"));
	requestRdbDataAction->setEnabled(false);
	connect(requestRdbDataAction, SIGNAL(triggered()), this, SLOT(requestRdbData()));

	requestTopoDataAction = new QAction(QIcon(":/images/requestTopo.png"), QStringLiteral("请求拓扑数据(Ice)"), this);
	requestTopoDataAction->setStatusTip(QStringLiteral("通过Ice请求拓扑数据"));
	requestTopoDataAction->setEnabled(false);
	connect(requestTopoDataAction, SIGNAL(triggered()), this, SLOT(requestTopoData()));

	subscriberRdbRequestAction = new QAction(QIcon(":/images/subscribeRdbRequest.png"), QStringLiteral("订阅实时数据请求"), this);
	subscriberRdbRequestAction->setStatusTip(QStringLiteral("订阅实时数据请求"));
	subscriberRdbRequestAction->setEnabled(false);
	connect(subscriberRdbRequestAction, SIGNAL(triggered()), this, SLOT(subscriberRdbRequest()));

	subscriberRdbRespondAction = new QAction(QIcon(":/images/subscribeRdbRespond.png"), QStringLiteral("订阅实时数据响应"), this);
	subscriberRdbRespondAction->setStatusTip(QStringLiteral("订阅实时数据响应"));
	subscriberRdbRespondAction->setEnabled(false);
	connect(subscriberRdbRespondAction, SIGNAL(triggered()), this, SLOT(subscriberRdbRespond()));

	subscriberAlarmDataAction = new QAction(QIcon(":/images/subscribeAlarmData.png"), QStringLiteral("订阅报警数据"), this);
	subscriberAlarmDataAction->setStatusTip(QStringLiteral("订阅报警数据"));
	subscriberAlarmDataAction->setEnabled(false);
	connect(subscriberAlarmDataAction, SIGNAL(triggered()), this, SLOT(subscriberAlarmData()));

	subscriberFepDataAction = new QAction(QIcon(":/images/subscribeFepData.png"), QStringLiteral("订阅前置机数据请求"), this);
	subscriberFepDataAction->setStatusTip(QStringLiteral("订阅前置机数据请求"));
	subscriberFepDataAction->setEnabled(false);
	connect(subscriberFepDataAction, SIGNAL(triggered()), this, SLOT(subscriberFepData()));

	subscriberYkFepAction = new QAction(QIcon(":/images/subscribeFepData.png"), QStringLiteral("订阅遥控请求"), this);
	subscriberYkFepAction->setStatusTip(QStringLiteral("订阅遥控请求"));
	subscriberYkFepAction->setEnabled(false);
	connect(subscriberYkFepAction, SIGNAL(triggered()), this, SLOT(subscriberYkFep()));

	subscriberYkAppAction = new QAction(QIcon(":/images/subscribeFepData.png"), QStringLiteral("订阅遥控响应"), this);
	subscriberYkAppAction->setStatusTip(QStringLiteral("订阅遥控响应"));
	subscriberYkAppAction->setEnabled(false);
	connect(subscriberYkAppAction, SIGNAL(triggered()), this, SLOT(subscriberYkApp()));

	ykSelectAction = new QAction(QIcon(":/images/subscribeFepData.png"), QStringLiteral("遥控选择"), this);
	ykSelectAction->setStatusTip(QStringLiteral("遥控选择"));
	ykSelectAction->setEnabled(false);
	connect(ykSelectAction, SIGNAL(triggered()), this, SLOT(ykSelect()));
	
	clearAction = new QAction(QIcon(":/images/clear.png"), QStringLiteral("清空文本"), this);
	clearAction->setStatusTip(QStringLiteral("清空文本"));
	connect(clearAction, SIGNAL(triggered()), this, SLOT(clearTextEdit()));

	helpAction = new QAction(QIcon(":/images/about.png"), QStringLiteral("关于"), this);
	helpAction->setStatusTip(QStringLiteral("关于模拟机程序"));
	connect(helpAction, SIGNAL(triggered()), this, SLOT(about()));
}

void WorkStationFrame::createMenus()
{
	fileMenu = menuBar()->addMenu(QStringLiteral("文件"));
	fileMenu->addAction(startServerAction);
	fileMenu->addAction(stopServerAction);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);

	operMenu = menuBar()->addMenu(QStringLiteral("操作"));
	QMenu* iceStormMenu = operMenu->addMenu(QIcon(":/images/icestormmenu.png"), QStringLiteral("IceStorm"));
	iceStormMenu->addAction(requestStormRdbDataAction);
	iceStormMenu->addAction(requestStormTopoDataAction);
	QMenu* iceMenu = operMenu->addMenu(QIcon(":/images/icemenu.png"), QStringLiteral("Ice"));
	iceMenu->addAction(requestRdbDataAction);
	iceMenu->addAction(requestTopoDataAction);
	QMenu* subscriberMenu = operMenu->addMenu(QIcon(":/images/icemenu.png"), QStringLiteral("IceStorm Subscriber"));
	subscriberMenu->addAction(subscriberRdbRequestAction);
	subscriberMenu->addAction(subscriberRdbRespondAction);
	subscriberMenu->addAction(subscriberAlarmDataAction);
	subscriberMenu->addAction(subscriberFepDataAction);
	subscriberMenu->addAction(subscriberYkFepAction);
	subscriberMenu->addAction(subscriberYkAppAction);
	subscriberMenu->addSeparator();
	subscriberMenu->addAction(ykSelectAction);
	operMenu->addSeparator();
	operMenu->addAction(clearAction);

	helpMenu = menuBar()->addMenu(QStringLiteral("帮助"));
	helpMenu->addAction(helpAction);
}

void WorkStationFrame::createToolBars()
{
	fileToolBar = addToolBar(QStringLiteral("文件"));
	fileToolBar->addAction(startServerAction);
	fileToolBar->addAction(stopServerAction);
	fileToolBar->addSeparator();
	fileToolBar->addAction(exitAction);

	operToolBar = addToolBar(QStringLiteral("操作"));
	operToolBar->addAction(requestStormRdbDataAction);
	operToolBar->addAction(requestStormTopoDataAction);
	operToolBar->addSeparator();
	operToolBar->addAction(requestRdbDataAction);
	operToolBar->addAction(requestTopoDataAction);
	operToolBar->addSeparator();
	operToolBar->addAction(subscriberRdbRequestAction);
	operToolBar->addAction(subscriberRdbRespondAction);
	operToolBar->addAction(subscriberAlarmDataAction);
	operToolBar->addAction(subscriberFepDataAction);
	operToolBar->addAction(subscriberYkFepAction);
	operToolBar->addAction(subscriberYkAppAction);
	operToolBar->addSeparator();
	operToolBar->addAction(ykSelectAction);
	operToolBar->addSeparator();
	operToolBar->addAction(clearAction);
}

void WorkStationFrame::createStatusBar()
{
	statusBar()->showMessage(QStringLiteral("准备就绪"));
}

void WorkStationFrame::createConnects()
{
	qRegisterMetaType<OperationInfo>("OperationInfo");
	connect(this, SIGNAL(serverStarted(bool)), this, SLOT(updateActions(bool)));
	connect(this, SIGNAL(requestCompleteData()), m_workStationServerThreadPtr, SLOT(requestCompleteData()));
	connect(this, SIGNAL(selectCompleteData()), m_workStationServerThreadPtr, SLOT(selectCompleteData()));
	connect(this, SIGNAL(subscriberRdbRequestSignal(bool)), m_workStationServerThreadPtr, SLOT(subscriberRdbRequest(bool)));
	connect(this, SIGNAL(subscriberRdbRespondSignal(bool)), m_workStationServerThreadPtr, SLOT(subscriberRdbRespond(bool)));
	connect(this, SIGNAL(subscriberAlarmDataSignal(bool)), m_workStationServerThreadPtr, SLOT(subscriberAlarmData(bool)));
	connect(this, SIGNAL(subscriberFepDataSignal(bool)), m_workStationServerThreadPtr, SLOT(subscriberFepData(bool)));
	connect(this, SIGNAL(subscriberYkFepSignal(bool)), m_workStationServerThreadPtr, SLOT(subscriberYkFep(bool)));
	connect(this, SIGNAL(subscriberYkAppSignal(bool)), m_workStationServerThreadPtr, SLOT(subscriberYkApp(bool)));
	connect(this, SIGNAL(ykSelectSignal(bool)), m_workStationServerThreadPtr, SLOT(ykSelect(bool)));
	connect(m_workStationServerThreadPtr, &WorkStationServerThread::executeOperation, 
		this, &WorkStationFrame::updateTableWidget);
	connect(m_workStationServerThreadPtr, &WorkStationServerThread::outputReceiveData, 
		this, &WorkStationFrame::updateTextEdit);
}

void WorkStationFrame::updateTableWidget( const OperationInfo& info )
{
	if (tableWidget)
	{
		int count = tableWidget->rowCount();
		tableWidget->insertRow(count);
		for (int i = 0; i < Header_Count; ++i)
		{
			tableWidget->setItem(count, i, new QTableWidgetItem(info.getContent(i)));
		}

		tableWidget->resizeColumnsToContents();
	}
}

void WorkStationFrame::updateTextEdit( const QString& text )
{
	textEdit->moveCursor(QTextCursor::End);
	textEdit->insertPlainText(text);
	textEdit->insertPlainText("\n");
	textEdit->moveCursor(QTextCursor::End);
}

void WorkStationFrame::updateActions( bool serverStarted )
{
	startServerAction->setEnabled(!serverStarted);
	stopServerAction->setEnabled(serverStarted);
	requestStormRdbDataAction->setEnabled(serverStarted);
	requestStormTopoDataAction->setEnabled(serverStarted);
	requestRdbDataAction->setEnabled(serverStarted);
	requestTopoDataAction->setEnabled(serverStarted);
	subscriberRdbRequestAction->setEnabled(serverStarted);
	subscriberRdbRespondAction->setEnabled(serverStarted);
	subscriberAlarmDataAction->setEnabled(serverStarted);
	subscriberFepDataAction->setEnabled(serverStarted);
	subscriberYkFepAction->setEnabled(serverStarted);
	subscriberYkAppAction->setEnabled(serverStarted);
	ykSelectAction->setEnabled(serverStarted);
}

void WorkStationFrame::startServer()
{
	QThread* threadPtr = new QThread;
	m_workStationServerPtr->setThreadPtr(m_workStationServerThreadPtr);
	m_workStationServerPtr->moveToThread(threadPtr);
	connect(threadPtr, SIGNAL(started()), m_workStationServerPtr, SLOT(startServer()));
	connect(m_workStationServerPtr, &WorkStationServer::executeOperation, this, &WorkStationFrame::updateTableWidget);
	connect(m_workStationServerThreadPtr, &WorkStationServerThread::executeOperation, this, &WorkStationFrame::updateTableWidget);
	threadPtr->start();

	emit serverStarted(true);
}

void WorkStationFrame::stopServer()
{
	emit serverStarted(false);
}

void WorkStationFrame::requestStormRdbData()
{
	emit requestCompleteData();
}

void WorkStationFrame::requestStormTopoData()
{

}

void WorkStationFrame::requestRdbData()
{
	emit selectCompleteData();
}

void WorkStationFrame::requestTopoData()
{

}

void WorkStationFrame::subscriberRdbRequest()
{
	QString text = subscriberRdbRequestAction->text();
	if (text.contains(QStringLiteral("取消")))
	{
		subscriberRdbRequestAction->setText(QStringLiteral("订阅实时数据请求"));
		subscriberRdbRequestAction->setStatusTip(QStringLiteral("订阅实时数据请求"));

		emit subscriberRdbRequestSignal(true);
	}
	else
	{
		subscriberRdbRequestAction->setText(QStringLiteral("取消订阅实时数据请求"));
		subscriberRdbRequestAction->setStatusTip(QStringLiteral("取消订阅实时数据请求"));

		emit subscriberRdbRequestSignal(false);
	}
}


void WorkStationFrame::subscriberRdbRespond()
{
	QString text = subscriberRdbRespondAction->text();
	if (text.contains(QStringLiteral("取消")))
	{
		subscriberRdbRespondAction->setText(QStringLiteral("订阅实时数据响应"));
		subscriberRdbRespondAction->setStatusTip(QStringLiteral("订阅实时数据响应"));

		emit subscriberRdbRespondSignal(true);
	}
	else
	{
		subscriberRdbRespondAction->setText(QStringLiteral("取消订阅实时数据响应"));
		subscriberRdbRespondAction->setStatusTip(QStringLiteral("取消订阅实时数据响应"));

		emit subscriberRdbRespondSignal(false);
	}
}

void WorkStationFrame::subscriberAlarmData()
{
	QString text = subscriberAlarmDataAction->text();
	if (text.contains(QStringLiteral("取消")))
	{
		subscriberAlarmDataAction->setText(QStringLiteral("订阅报警数据"));
		subscriberAlarmDataAction->setStatusTip(QStringLiteral("订阅报警数据"));

		emit subscriberAlarmDataSignal(true);
	}
	else
	{
		subscriberAlarmDataAction->setText(QStringLiteral("取消订阅报警数据"));
		subscriberAlarmDataAction->setStatusTip(QStringLiteral("取消订阅报警数据"));

		emit subscriberAlarmDataSignal(false);
	}
}

void WorkStationFrame::subscriberFepData()
{
	QString text = subscriberFepDataAction->text();
	if (text.contains(QStringLiteral("取消")))
	{
		subscriberFepDataAction->setText(QStringLiteral("订阅前置机数据"));
		subscriberFepDataAction->setStatusTip(QStringLiteral("订阅前置机数据"));

		emit subscriberFepDataSignal(true);
	}
	else
	{
		subscriberFepDataAction->setText(QStringLiteral("取消订阅前置机数据"));
		subscriberFepDataAction->setStatusTip(QStringLiteral("取消订阅前置机数据"));

		emit subscriberFepDataSignal(false);
	}
}


void WorkStationFrame::subscriberYkFep()
{
	QString text = subscriberYkFepAction->text();
	if (text.contains(QStringLiteral("取消")))
	{
		subscriberYkFepAction->setText(QStringLiteral("订阅遥控请求"));
		subscriberYkFepAction->setStatusTip(QStringLiteral("订阅遥控请求"));

		emit subscriberYkFepSignal(true);
	}
	else
	{
		subscriberYkFepAction->setText(QStringLiteral("取消订阅遥控请求"));
		subscriberYkFepAction->setStatusTip(QStringLiteral("取消订阅遥控请求"));

		emit subscriberYkFepSignal(false);
	}
}

void WorkStationFrame::subscriberYkApp()
{
	QString text = subscriberYkAppAction->text();
	if (text.contains(QStringLiteral("取消")))
	{
		subscriberYkAppAction->setText(QStringLiteral("订阅遥控响应"));
		subscriberYkAppAction->setStatusTip(QStringLiteral("订阅遥控响应"));

		emit subscriberYkAppSignal(true);
	}
	else
	{
		subscriberYkAppAction->setText(QStringLiteral("取消订阅遥控响应"));
		subscriberYkAppAction->setStatusTip(QStringLiteral("取消订阅遥控响应"));

		emit subscriberYkAppSignal(false);
	}
}

void WorkStationFrame::ykSelect()
{
	QString text = ykSelectAction->text();
	if (text.contains(QStringLiteral("取消")))
	{
		ykSelectAction->setText(QStringLiteral("遥控选择"));
		ykSelectAction->setStatusTip(QStringLiteral("遥控选择"));

		emit ykSelectSignal(true);
	}
	else
	{
		ykSelectAction->setText(QStringLiteral("取消遥控选择"));
		ykSelectAction->setStatusTip(QStringLiteral("取消遥控选择"));

		emit ykSelectSignal(false);
	}
}

void WorkStationFrame::clearTextEdit()
{
	if (textEdit)
	{
		textEdit->clear();
	}
}

void WorkStationFrame::about()
{
	QMessageBox::information(this, QStringLiteral("工作站模拟机"), 
		QStringLiteral("提供模拟工作站请求数据的功能"));
}