#include "subscriberfileframe.h"

#include <QtWidgets/QtWidgets>

const QString	TITLE = QStringLiteral("模拟订阅文件");

SubscriberFileFrame::SubscriberFileFrame(QWidget *parent)
	: QMainWindow(parent)
{
	m_subscribering = false;

	createWidgets();
	createActions();
	createConnectes();
	createMenus();
	createToolBars();
	createStatusBar();
}

SubscriberFileFrame::~SubscriberFileFrame()
{

}

void SubscriberFileFrame::setTransFileServer( const QSharedPointer<TransferFileServer>& server )
{
	m_transFileServer = server;
}

void SubscriberFileFrame::createWidgets()
{
	textEdit = new QTextEdit();
	setCentralWidget(textEdit);

	setWindowTitle(QStringLiteral("%1").arg(TITLE));
	setWindowIcon(QIcon(":/images/app.png"));
	resize(800, 600);
}

void SubscriberFileFrame::createActions()
{
	closeAction = new QAction(QIcon(":/images/close.png"), QStringLiteral("关闭程序"), this);
	closeAction->setStatusTip(QStringLiteral("关闭程序"));
	connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

	subscriberAction = new QAction(QIcon(":/images/subscriber.png"), QStringLiteral("订阅文件"), this);
	subscriberAction->setStatusTip(QStringLiteral("订阅文件"));
	connect(subscriberAction, SIGNAL(triggered()), this, SLOT(subscriberFile()));

	clearAction = new QAction(QIcon(":/images/clear.png"), QStringLiteral("清空文本"), this);
	clearAction->setStatusTip(QStringLiteral("清空文本"));
	connect(clearAction, SIGNAL(triggered()), this, SLOT(clearTextEdit()));
}

void SubscriberFileFrame::createConnectes()
{

}

void SubscriberFileFrame::createMenus()
{
	fileMenu = menuBar()->addMenu(QStringLiteral("文件"));
	fileMenu->addAction(closeAction);

	operMenu = menuBar()->addMenu(QStringLiteral("操作"));
	operMenu->addAction(subscriberAction);
	operMenu->addAction(clearAction);
}

void SubscriberFileFrame::createToolBars()
{
	fileToolBar = addToolBar(QStringLiteral("文件"));
	fileToolBar->addAction(closeAction);

	operToolBar = addToolBar(QStringLiteral("操作"));
	operToolBar->addAction(subscriberAction);
	operToolBar->addAction(clearAction);
}

void SubscriberFileFrame::createStatusBar()
{
	statusBar()->showMessage(QStringLiteral("准备就绪"));
}

void SubscriberFileFrame::updateActions()
{

}

void SubscriberFileFrame::subscriberFile()
{
	m_subscribering = !m_subscribering;
	if (m_subscribering)
	{
		// 开始订阅文件

		setWindowTitle(QStringLiteral("%1: %2").arg(TITLE).arg(QStringLiteral("正在订阅文件...")));
		subscriberAction->setText(QStringLiteral("取消订阅文件"));

		m_transFileServer->subscriberFile(true);
	}
	else
	{
		// 停止订阅文件

		setWindowTitle(QStringLiteral("%1").arg(TITLE));
		subscriberAction->setText(QStringLiteral("订阅文件"));

		m_transFileServer->subscriberFile(false);
	}
}

void SubscriberFileFrame::clearTextEdit()
{
	if (textEdit)
	{
		textEdit->clear();
	}
}

void SubscriberFileFrame::updateTextEdit( QString text )
{
	if (textEdit)
	{
		textEdit->moveCursor(QTextCursor::End);
		textEdit->insertPlainText(text);
		textEdit->insertPlainText("\n");
		textEdit->moveCursor(QTextCursor::End);
	}
}
