#include "amsframe.h"
#include "AmsServer.h"
#include "AmsServerThread.h"
#include "OperationInfo.h"
#include "TableHeader.h"

#include <QtWidgets/QtWidgets>

AmsFrame::AmsFrame(QWidget *parent)
	: QMainWindow(parent)
{
	createWidgets();

	createActions();

	createMenus();
	createToolbar();
	createStatusBar();
}

AmsFrame::~AmsFrame()
{

}

void AmsFrame::setParam( int argc, char* argv[] )
{
	m_argc = argc;
	m_argv = argv;
}

void AmsFrame::createWidgets()
{
	tableWidget = new QTableWidget();
	QStringList labels = TableHeader::getTableHeaderLabels();
	tableWidget->setColumnCount(labels.count());
	tableWidget->setHorizontalHeaderLabels(labels);
	tableWidget->setShowGrid(true);
	tableWidget->setWordWrap(true);
	tableWidget->setAlternatingRowColors(true);
	setCentralWidget(tableWidget);

	resize(800, 600);
	setWindowTitle(QStringLiteral("AMS模拟机(返回主角色)"));
	setWindowIcon(QIcon(":/images/ams.png"));
}

void AmsFrame::createActions()
{
	exitAction = new QAction(QIcon(":/images/exit.png"), QStringLiteral("退出"), this);
	exitAction->setShortcuts(QKeySequence::Quit);
	exitAction->setStatusTip(QStringLiteral("退出程序"));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

	setMasterAction = new QAction(QIcon(":/images/master.png"), QStringLiteral("返回主角色"), this);
	setMasterAction->setStatusTip(QStringLiteral("返回主角色"));
	setMasterAction->setEnabled(false);
	connect(setMasterAction, SIGNAL(triggered()), this, SLOT(setMasterRole()));

	setSlaveAction = new QAction(QIcon(":/images/about.png"), QStringLiteral("返回备角色"), this);
	setSlaveAction->setStatusTip(QStringLiteral("返回备角色"));
	connect(setSlaveAction, SIGNAL(triggered()), this, SLOT(setSlaveRole()));

	helpAction = new QAction(QIcon(":/images/about.png"), QStringLiteral("关于"), this);
	helpAction->setStatusTip(QStringLiteral("关于程序的介绍"));
	connect(helpAction, SIGNAL(triggered()), this, SLOT(about()));
}

void AmsFrame::createMenus()
{
	fileMenu = menuBar()->addMenu(QStringLiteral("文件"));
	fileMenu->addAction(exitAction);

	operMenu = menuBar()->addMenu(QStringLiteral("操作"));
	operMenu->addAction(setMasterAction);
	operMenu->addAction(setSlaveAction);
}

void AmsFrame::createToolbar()
{
	fileToolbar = addToolBar(QStringLiteral("文件"));
	fileToolbar->addAction(exitAction);

	operToolbar = addToolBar(QStringLiteral("操作"));
	operToolbar->addAction(setMasterAction);
	operToolbar->addAction(setSlaveAction);
}

void AmsFrame::createStatusBar()
{
	statusBar()->showMessage(QStringLiteral("准备就绪"));
}

void AmsFrame::hasSetRole(bool isMaster)
{
	setMasterAction->setEnabled(!isMaster);
	setSlaveAction->setEnabled(isMaster);
	if (isMaster)
	{
		setWindowTitle(QStringLiteral("AMS模拟机(返回主角色)"));
	}
	else
	{
		setWindowTitle(QStringLiteral("AMS模拟机(返回备角色)"));
	}
}

// 开始执行服务
void AmsFrame::startServer()
{
	qRegisterMetaType<OperationInfo>("OperationInfo");

	AmsServerThread* thread = new AmsServerThread();
	thread->setParam(m_argc, m_argv);
	connect(thread, &AmsServerThread::executeOperation, this, &AmsFrame::updateTableWidget);
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	connect(this, SIGNAL(setRole(QString)), thread, SLOT(setRole(QString)));
	thread->start();
}

void AmsFrame::about()
{
	QMessageBox::about(this, QStringLiteral("AMS模拟机"), 
		QStringLiteral("模拟AMS管理数据服务器的功能"));
}

void AmsFrame::updateTableWidget( const OperationInfo& info )
{
	if (tableWidget)
	{
		int count = tableWidget->rowCount();
		tableWidget->insertRow(count);
		for (int i = 0; i < Header_Count; ++i)
		{
			tableWidget->setItem(count, i, new QTableWidgetItem(info.getContent(i)));
		}
		tableWidget->resizeColumnToContents(Header_OperTime);
	}
}

void AmsFrame::setMasterRole()
{
	hasSetRole(true);
	emit setRole(ROLE_MASTER);
}

void AmsFrame::setSlaveRole()
{
	hasSetRole(false);
	emit setRole(ROLE_SLAVE);
}
