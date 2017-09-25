#include "fepframe.h"
#include "fepserver.h"
#include "fepserverthread.h"

#include <QtWidgets/QtWidgets>
#include <QtCore/QThread>

FepFrame::FepFrame(int argc, char* argv[], QWidget *parent)
	: QMainWindow(parent)
{
	m_fepServerPtr = new FepServer(argc, argv);
	m_fepServerThreadPtr = new FepServerThread;

	createWidgets();

	createActions();
	createMenus();
	createToolBars();
	createStatusBar();
}

FepFrame::~FepFrame()
{

}

void FepFrame::startServer()
{
	QThread* threadPtr = new QThread;
	m_fepServerPtr->setThreadPtr(m_fepServerThreadPtr);
	m_fepServerPtr->moveToThread(threadPtr);
	connect(threadPtr, SIGNAL(started()), m_fepServerPtr, SLOT(startServer()));
	connect(m_fepServerPtr, &FepServer::executeOperation, this, &FepFrame::updateListWidget);
	connect(m_fepServerThreadPtr, &FepServerThread::executeOperation, this, &FepFrame::updateListWidget);
	threadPtr->start();
}

void FepFrame::createWidgets()
{
	listWidget = new QListWidget;
	listWidget->setAlternatingRowColors(true);
	listWidget->setWordWrap(true);
	setCentralWidget(listWidget);

	resize(800, 600);
	setWindowTitle(QStringLiteral("前置机模拟机"));
	setWindowIcon(QIcon(":/images/fep.png"));
}

void FepFrame::createActions()
{
	exitAction = new QAction(QIcon(":/images/exit.png"), QStringLiteral("退出"), this);
	exitAction->setShortcut(QKeySequence::Quit);
	exitAction->setStatusTip(QStringLiteral("退出模拟机程序"));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

	processDataAction = new QAction(QIcon(":/images/data.png"), QStringLiteral("发布数据"), this);
	processDataAction->setStatusTip(QStringLiteral("发布前置机数据"));
	connect(processDataAction, SIGNAL(triggered()), m_fepServerThreadPtr, SLOT(processData()));

	helpAction = new QAction(QIcon(":/images/about.png"), QStringLiteral("关于"), this);
	helpAction->setStatusTip(QStringLiteral("关于模拟机程序"));
	connect(helpAction, SIGNAL(triggered()), this, SLOT(about()));
}

void FepFrame::createMenus()
{
	fileMenu = menuBar()->addMenu(QStringLiteral("文件"));
	fileMenu->addAction(exitAction);

	operMenu = menuBar()->addMenu(QStringLiteral("操作"));
	operMenu->addAction(processDataAction);

	helpMenu = menuBar()->addMenu(QStringLiteral("帮助"));
	helpMenu->addAction(helpAction);
}

void FepFrame::createToolBars()
{
	fileToolBar = addToolBar(QStringLiteral("文件"));
	fileToolBar->addAction(exitAction);

	operToolBar = addToolBar(QStringLiteral("操作"));
	operToolBar->addAction(processDataAction);
}

void FepFrame::createStatusBar()
{
	statusBar()->showMessage(QStringLiteral("准备就绪"));
}

void FepFrame::about()
{
	QMessageBox::information(this, QStringLiteral("前置机模拟机"), 
			QStringLiteral("提供模拟前置机发送数据的功能"));
}

void FepFrame::updateListWidget( const QString& text )
{
	if (listWidget)
	{
		listWidget->addItem(text);
	}
}
