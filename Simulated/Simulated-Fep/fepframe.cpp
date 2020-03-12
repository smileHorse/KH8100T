#include "fepframe.h"
#include "fepserver.h"
#include "fepserverthread.h"
#include "faultEventDlg.h"
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

	processYcDataAction = new QAction(QIcon(":/images/ycdata.png"), QStringLiteral("发布遥测数据"), this);
	processYcDataAction->setStatusTip(QStringLiteral("前置机发布遥测数据"));
	connect(processYcDataAction, SIGNAL(triggered()), m_fepServerThreadPtr, SLOT(processYcData()));
	
	processYxDataAction = new QAction(QIcon(":/images/yxdata.png"), QStringLiteral("发布遥信数据"), this);
	processYxDataAction->setStatusTip(QStringLiteral("前置机发布遥信数据"));
	connect(processYxDataAction, SIGNAL(triggered()), m_fepServerThreadPtr, SLOT(processYxData()));

	processDdDataAction = new QAction(QIcon(":/images/dddata.png"), QStringLiteral("发布电度数据"), this);
	processDdDataAction->setStatusTip(QStringLiteral("前置机发布电度数据"));
	connect(processDdDataAction, SIGNAL(triggered()), m_fepServerThreadPtr, SLOT(processDdData()));

	processUnitStateDataAction = new QAction(QIcon(":/images/unitstatedata.png"), QStringLiteral("发布终端状态"), this);
	processUnitStateDataAction->setStatusTip(QStringLiteral("前置机发布终端状态"));
	connect(processUnitStateDataAction, SIGNAL(triggered()), this, SLOT(processUnitStateData()));

	processDLFaultAction = new QAction(QIcon(":/images/dlfault.png"), QStringLiteral("发布短路故障事项"), this);
	processDLFaultAction->setStatusTip(QStringLiteral("前置机发布短路故障事项"));
	connect(processDLFaultAction, SIGNAL(triggered()), m_fepServerThreadPtr, SLOT(processDLFault()));

	processJDFaultAction = new QAction(QIcon(":/images/jdfault.png"), QStringLiteral("发布接地故障事项"), this);
	processJDFaultAction->setStatusTip(QStringLiteral("前置机发布接地故障事项"));
	connect(processJDFaultAction, SIGNAL(triggered()), m_fepServerThreadPtr, SLOT(processJDFault()));

	processYxTypeEventAction = new QAction(QIcon(":/images/yxtype.png"), QStringLiteral("发布遥信变位事项"), this);
	processYxTypeEventAction->setStatusTip(QStringLiteral("发布前置机遥信变位事项"));
	connect(processYxTypeEventAction, SIGNAL(triggered()), this, SLOT(processYxTypeEvent()));

	processSoeTypeEventAction = new QAction(QIcon(":/images/soetype.png"), QStringLiteral("发布SOE事项"), this);
	processSoeTypeEventAction->setStatusTip(QStringLiteral("发布前置机SOE事项"));
	connect(processSoeTypeEventAction, SIGNAL(triggered()), this, SLOT(processSoeTypeEvent()));

	processUnitTypeEventAction = new QAction(QIcon(":/images/unittype.png"), QStringLiteral("发布单元事项"), this);
	processUnitTypeEventAction->setStatusTip(QStringLiteral("发布前置机单元事项"));
	connect(processUnitTypeEventAction, SIGNAL(triggered()), m_fepServerThreadPtr, SLOT(processUnitTypeEvent()));

	processUnitChannelEventAction = new QAction(QIcon(":/images/unittype.png"), QStringLiteral("发布通道事项"), this);
	processUnitChannelEventAction->setStatusTip(QStringLiteral("发布前置机通道事项"));
	connect(processUnitChannelEventAction, SIGNAL(triggered()), m_fepServerThreadPtr, SLOT(processUnitChannelEvent()));

	processProTypeEventAction = new QAction(QIcon(":/images/protype.png"), QStringLiteral("发布保护事项"), this);
	processProTypeEventAction->setStatusTip(QStringLiteral("发布前置机保护事项"));
	//connect(processProTypeEventAction, SIGNAL(triggered()), m_fepServerThreadPtr, SLOT(processProTypeEvent()));
	//connect(processProTypeEventAction, SIGNAL(triggered()), this, SLOT(configFaultEvent()));
	connect(processProTypeEventAction, SIGNAL(triggered()), this, SLOT(processProTypeEvent()));
	
	processWaveAction = new QAction(QIcon(":/images/wave.png"), QStringLiteral("发布录波事项"), this);
	processWaveAction->setStatusTip(QStringLiteral("发布前置机录波事项"));
	connect(processWaveAction, SIGNAL(triggered()), m_fepServerThreadPtr, SLOT(processWave()));

	clearAction = new QAction(QIcon(":/images/clear.png"), QStringLiteral("清空文本"), this);
	clearAction->setStatusTip(QStringLiteral("清空文本控件"));
	connect(clearAction, SIGNAL(triggered()), this, SLOT(clearTextEdit()));
	
	helpAction = new QAction(QIcon(":/images/about.png"), QStringLiteral("关于"), this);
	helpAction->setStatusTip(QStringLiteral("关于模拟机程序"));
	connect(helpAction, SIGNAL(triggered()), this, SLOT(about()));
}

void FepFrame::createMenus()
{
	fileMenu = menuBar()->addMenu(QStringLiteral("文件"));
	fileMenu->addAction(exitAction);

	operMenu = menuBar()->addMenu(QStringLiteral("操作"));
	operMenu->addAction(processYcDataAction);
	operMenu->addAction(processYxDataAction);
	operMenu->addAction(processDdDataAction);
	operMenu->addAction(processUnitStateDataAction);
	operMenu->addSeparator();
	operMenu->addAction(processDLFaultAction);
	operMenu->addAction(processJDFaultAction);
	operMenu->addSeparator();
	operMenu->addAction(processYxTypeEventAction);
	operMenu->addAction(processSoeTypeEventAction);
	operMenu->addAction(processUnitTypeEventAction);
	operMenu->addAction(processUnitChannelEventAction);
	operMenu->addAction(processProTypeEventAction);
	operMenu->addSeparator();
	operMenu->addAction(processWaveAction);
	operMenu->addSeparator();
	operMenu->addAction(clearAction);

	helpMenu = menuBar()->addMenu(QStringLiteral("帮助"));
	helpMenu->addAction(helpAction);
}

void FepFrame::createToolBars()
{
	fileToolBar = addToolBar(QStringLiteral("文件"));
	fileToolBar->addAction(exitAction);

	operToolBar = addToolBar(QStringLiteral("操作"));
	operToolBar->addAction(processYcDataAction);
	operToolBar->addAction(processYxDataAction);
	operToolBar->addAction(processDdDataAction);
	operToolBar->addAction(processUnitStateDataAction);
	operToolBar->addSeparator();
	operToolBar->addAction(processDLFaultAction);
	operToolBar->addAction(processJDFaultAction);
	operToolBar->addSeparator();
	operToolBar->addAction(processYxTypeEventAction);
	operToolBar->addAction(processSoeTypeEventAction);
	operToolBar->addAction(processUnitTypeEventAction);
	operToolBar->addAction(processUnitChannelEventAction);
	operToolBar->addAction(processProTypeEventAction);
	operToolBar->addSeparator();
	operToolBar->addAction(processWaveAction);
	operToolBar->addSeparator();
	operToolBar->addAction(clearAction);
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
	textEdit->moveCursor(QTextCursor::End);
	textEdit->insertPlainText(text);
	textEdit->insertPlainText("\n");
	textEdit->moveCursor(QTextCursor::End);
}

void FepFrame::clearTextEdit()
{
	if (textEdit)
	{
		textEdit->clear();
	}
}

void FepFrame::processUnitStateData()
{
	QString text = processUnitStateDataAction->text();
	if (text.contains(QStringLiteral("取消")))
	{
		processUnitStateDataAction->setText(QStringLiteral("发布终端状态"));
		processUnitStateDataAction->setStatusTip(QStringLiteral("发布终端状态"));

		m_fepServerThreadPtr->processUnitStateData(false);
	}
	else
	{
		processUnitStateDataAction->setText(QStringLiteral("取消发布终端状态"));
		processUnitStateDataAction->setStatusTip(QStringLiteral("取消发布终端状态"));

		m_fepServerThreadPtr->processUnitStateData(true);
	}
}

void FepFrame::processYxTypeEvent()
{
	QString text = processYxTypeEventAction->text();
	if (text.contains(QStringLiteral("取消")))
	{
		processYxTypeEventAction->setText(QStringLiteral("发布遥信变位事项"));
		processYxTypeEventAction->setStatusTip(QStringLiteral("发布遥信变位事项"));

		m_fepServerThreadPtr->processYxTypeEvent(false);
	}
	else
	{
		processYxTypeEventAction->setText(QStringLiteral("取消发布遥信变位事项"));
		processYxTypeEventAction->setStatusTip(QStringLiteral("取消发布遥信变位事项"));

		m_fepServerThreadPtr->processYxTypeEvent(true);
	}
}

void FepFrame::processSoeTypeEvent()
{
	QString text = processSoeTypeEventAction->text();
	if (text.contains(QStringLiteral("取消")))
	{
		processSoeTypeEventAction->setText(QStringLiteral("发布SOE事项"));
		processSoeTypeEventAction->setStatusTip(QStringLiteral("发布SOE事项"));

		m_fepServerThreadPtr->processSoeTypeEvent(false);
	}
	else
	{
		processSoeTypeEventAction->setText(QStringLiteral("取消发布SOE事项"));
		processSoeTypeEventAction->setStatusTip(QStringLiteral("取消发布SOE事项"));

		m_fepServerThreadPtr->processSoeTypeEvent(true);
	}
}

void FepFrame::processProTypeEvent()
{
	QString text = processProTypeEventAction->text();
	if (text.contains(QStringLiteral("取消")))
	{
		processProTypeEventAction->setText(QStringLiteral("发布保护事项"));
		processProTypeEventAction->setStatusTip(QStringLiteral("发布保护事项"));

		m_fepServerThreadPtr->processProTypeEvent(false);
	}
	else
	{
		processProTypeEventAction->setText(QStringLiteral("取消发布保护事项"));
		processProTypeEventAction->setStatusTip(QStringLiteral("取消发布保护事项"));

		m_fepServerThreadPtr->processProTypeEvent(true);
	}
}

void FepFrame::configFaultEvent()
{
	CFaultEventDlg dlg(m_fepServerThreadPtr, this);
	dlg.exec();
}
