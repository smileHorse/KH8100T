#include "sqlframe.h"

#include <QtWidgets/QtWidgets>

#define THREAD_COUNT	10

SqlFrame::SqlFrame(QWidget *parent)
	: QMainWindow(parent)
{
	createWidgets();
	createActions();
	createConnectes();
	createMenus();
	createToolBars();
	createStatusBar();
}

SqlFrame::~SqlFrame()
{

}

void SqlFrame::createWidgets()
{
	textEdit = new QTextEdit();
	setCentralWidget(textEdit);

	setWindowTitle(QStringLiteral("MySql操作模拟程序"));
	setWindowIcon(QIcon(":/images/app.png"));
	resize(800, 600);
}

void SqlFrame::createActions()
{
	startAction = new QAction(QIcon(":/images/start.png"), QStringLiteral("开始执行"), this);
	startAction->setStatusTip(QStringLiteral("开始执行MySql操作"));
	connect(startAction, SIGNAL(triggered()), this, SLOT(startExecSql()));

	stopAction = new QAction(QIcon(":/images/stop.png"), QStringLiteral("停止执行"), this);
	stopAction->setStatusTip(QStringLiteral("停止执行MySql操作"));
	stopAction->setEnabled(false);
	connect(stopAction, SIGNAL(triggered()), this, SLOT(stopExecSql()));

	closeAction = new QAction(QIcon(":/images/close.png"), QStringLiteral("退出程序"), this);
	closeAction->setStatusTip(QStringLiteral("退出程序"));
	connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

	clearAction = new QAction(QIcon(":/images/clear.png"), QStringLiteral("清空文本"), this);
	clearAction->setStatusTip(QStringLiteral("清空文本"));
	connect(clearAction, SIGNAL(triggered()), this, SLOT(clearTextEdit()));
}

void SqlFrame::createConnectes()
{

}

void SqlFrame::createMenus()
{
	fileMenu = menuBar()->addMenu(QStringLiteral("文件"));
	fileMenu->addAction(startAction);
	fileMenu->addAction(stopAction);
	fileMenu->addSeparator();
	fileMenu->addAction(closeAction);

	operMenu = menuBar()->addMenu(QStringLiteral("操作"));
	operMenu->addAction(clearAction);
}

void SqlFrame::createToolBars()
{
	fileToolbar = addToolBar(QStringLiteral("文件"));
	fileToolbar->addAction(startAction);
	fileToolbar->addAction(stopAction);
	fileToolbar->addSeparator();
	fileToolbar->addAction(closeAction);

	operToolbar = addToolBar(QStringLiteral("操作"));
	operToolbar->addAction(clearAction);
}

void SqlFrame::createStatusBar()
{
	statusBar()->showMessage(QStringLiteral("准备就绪"));
}

void SqlFrame::updateExecAction( bool isStart )
{
	startAction->setEnabled(!isStart);
	stopAction->setEnabled(isStart);
}

void SqlFrame::startExecSql()
{
	updateExecAction(true);

	for (int i = 0; i < THREAD_COUNT; ++i)
	{
		QSharedPointer<CExecThread> execThreadPtr(new CExecThread);
		execThreadPtr->start();
		connect(execThreadPtr.data(), SIGNAL(outputOperInfo(QString)), this, SLOT(updateTextEdit(QString)));
		m_execThreads.push_back(execThreadPtr);
	}

}

void SqlFrame::stopExecSql()
{
	updateExecAction(false);

	for (int i = 0; i < m_execThreads.size(); ++i)
	{
		m_execThreads.at(i)->setStop(true);
	}

	for (int i = 0; i < m_execThreads.size(); )
	{
		if (m_execThreads.at(i)->isRunning())
		{
			continue;
		}
		++i;
	}
	m_execThreads.clear();
}

void SqlFrame::clearTextEdit()
{
	if (textEdit)
	{
		textEdit->clear();
	}
}

void SqlFrame::updateTextEdit( QString text )
{
	if (textEdit)
	{
		textEdit->moveCursor(QTextCursor::End);
		textEdit->insertPlainText(text);
		textEdit->moveCursor(QTextCursor::End);
	}
}
