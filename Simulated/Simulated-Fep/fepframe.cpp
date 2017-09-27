#include "fepframe.h"
#include "fepserver.h"
#include "fepserverthread.h"
#include "OperationInfo.h"
#include "TableHeader.h"

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
	qRegisterMetaType<OperationInfo>("OperationInfo");

	QThread* threadPtr = new QThread;
	m_fepServerPtr->setThreadPtr(m_fepServerThreadPtr);
	m_fepServerPtr->moveToThread(threadPtr);
	connect(threadPtr, SIGNAL(started()), m_fepServerPtr, SLOT(startServer()));
	connect(m_fepServerPtr, &FepServer::executeOperation, this, &FepFrame::updateTableWidget);
	connect(m_fepServerThreadPtr, &FepServerThread::executeOperation, this, &FepFrame::updateTableWidget);
	connect(m_fepServerThreadPtr, &FepServerThread::publishFepData, this, &FepFrame::updateTextEdit);
	threadPtr->start();
}

void FepFrame::createWidgets()
{
	tableWidget = new QTableWidget;
	QStringList headLabels = TableHeader::getTableHeaderLabels();
	tableWidget->setColumnCount(headLabels.count());
	tableWidget->setHorizontalHeaderLabels(headLabels);
	tableWidget->setShowGrid(true);
	tableWidget->setAlternatingRowColors(true);

	textEdit = new QTextEdit;
	textEdit->setReadOnly(false);

	QSplitter* splitter = new QSplitter(Qt::Vertical);
	splitter->addWidget(tableWidget);
	splitter->addWidget(textEdit);
	splitter->setStretchFactor(0, 1);
	splitter->setStretchFactor(1, 3);

	setCentralWidget(splitter);

	resize(1200, 800);
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
	processDataAction->setStatusTip(QStringLiteral("前置机发布数据"));
	connect(processDataAction, SIGNAL(triggered()), m_fepServerThreadPtr, SLOT(processData()));

	processDLFaultAction = new QAction(QIcon(":/images/dlfault.png"), QStringLiteral("发布短路故障事项"), this);
	processDLFaultAction->setStatusTip(QStringLiteral("前置机发布短路故障事项"));
	connect(processDLFaultAction, SIGNAL(triggered()), m_fepServerThreadPtr, SLOT(processDLFault()));

	processJDFaultAction = new QAction(QIcon(":/images/jdfault.png"), QStringLiteral("发布接地故障事项"), this);
	processJDFaultAction->setStatusTip(QStringLiteral("前置机发布接地故障事项"));
	connect(processJDFaultAction, SIGNAL(triggered()), m_fepServerThreadPtr, SLOT(processJDFault()));

	processYxTypeEventAction = new QAction(QIcon(":/images/yxtype.png"), QStringLiteral("发布遥信变位事项"), this);
	processYxTypeEventAction->setStatusTip(QStringLiteral("发布前置机遥信变位事项"));
	connect(processYxTypeEventAction, SIGNAL(triggered()), m_fepServerThreadPtr, SLOT(processYxTypeEvent()));

	processSoeTypeEventAction = new QAction(QIcon(":/images/soetype.png"), QStringLiteral("发布SOE事项"), this);
	processSoeTypeEventAction->setStatusTip(QStringLiteral("发布前置机SOE事项"));
	connect(processSoeTypeEventAction, SIGNAL(triggered()), m_fepServerThreadPtr, SLOT(processSoeTypeEvent()));

	processUnitTypeEventAction = new QAction(QIcon(":/images/unittype.png"), QStringLiteral("发布单元事项"), this);
	processUnitTypeEventAction->setStatusTip(QStringLiteral("发布前置机单元事项"));
	connect(processUnitTypeEventAction, SIGNAL(triggered()), m_fepServerThreadPtr, SLOT(processUnitTypeEvent()));

	processProTypeEventAction = new QAction(QIcon(":/images/protype.png"), QStringLiteral("发布保护事项"), this);
	processProTypeEventAction->setStatusTip(QStringLiteral("发布前置机保护事项"));
	connect(processProTypeEventAction, SIGNAL(triggered()), m_fepServerThreadPtr, SLOT(processProTypeEvent()));
	
	processWaveAction = new QAction(QIcon(":/images/wave.png"), QStringLiteral("发布录波事项"), this);
	processWaveAction->setStatusTip(QStringLiteral("发布前置机录波事项"));
	connect(processWaveAction, SIGNAL(triggered()), m_fepServerThreadPtr, SLOT(processWave()));
	
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
	operMenu->addSeparator();
	operMenu->addAction(processDLFaultAction);
	operMenu->addAction(processJDFaultAction);
	operMenu->addSeparator();
	operMenu->addAction(processYxTypeEventAction);
	operMenu->addAction(processSoeTypeEventAction);
	operMenu->addAction(processUnitTypeEventAction);
	operMenu->addAction(processProTypeEventAction);
	operMenu->addSeparator();
	operMenu->addAction(processWaveAction);

	helpMenu = menuBar()->addMenu(QStringLiteral("帮助"));
	helpMenu->addAction(helpAction);
}

void FepFrame::createToolBars()
{
	fileToolBar = addToolBar(QStringLiteral("文件"));
	fileToolBar->addAction(exitAction);

	operToolBar = addToolBar(QStringLiteral("操作"));
	operToolBar->addAction(processDataAction);
	operToolBar->addSeparator();
	operToolBar->addAction(processDLFaultAction);
	operToolBar->addAction(processJDFaultAction);
	operToolBar->addSeparator();
	operToolBar->addAction(processYxTypeEventAction);
	operToolBar->addAction(processSoeTypeEventAction);
	operToolBar->addAction(processUnitTypeEventAction);
	operToolBar->addAction(processProTypeEventAction);
	operToolBar->addSeparator();
	operToolBar->addAction(processWaveAction);
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

void FepFrame::updateTableWidget( const OperationInfo& info )
{
	if (tableWidget)
	{
		int nCount = tableWidget->rowCount();
		tableWidget->insertRow(nCount);
		for (int i = 0; i < Header_Count; ++i)
		{
			tableWidget->setItem(nCount, i, new QTableWidgetItem(info.getContent(i)));
		}

		tableWidget->resizeColumnToContents(Header_OperTime);
	}
}

void FepFrame::updateTextEdit( const QString& text )
{
	textEdit->insertPlainText(text);
	textEdit->insertPlainText("\n");
}
