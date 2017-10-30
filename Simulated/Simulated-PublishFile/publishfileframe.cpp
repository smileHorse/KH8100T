#include "publishfileframe.h"

#include <QtWidgets/QtWidgets>

const QString	TITLE = QStringLiteral("模拟发布文件");

PublishFileFrame::PublishFileFrame(QWidget *parent)
	: QMainWindow(parent)
{
	createWidgets();
	createActions();
	createConnectes();
	createMenus();
	createToolBars();
	createStatusBar();
}

PublishFileFrame::~PublishFileFrame()
{

}

void PublishFileFrame::setTransFileServer( const QSharedPointer<TransferFileServer>& server )
{
	m_transFileServer = server;
}

void PublishFileFrame::createWidgets()
{
	textEdit = new QTextEdit();
	setCentralWidget(textEdit);

	setWindowTitle(QStringLiteral("%1").arg(TITLE));
	setWindowIcon(QIcon(":/images/app.png"));
	resize(800, 600);
}

void PublishFileFrame::createActions()
{
	closeAction = new QAction(QIcon(":/images/close.png"), QStringLiteral("关闭程序"), this);
	closeAction->setStatusTip(QStringLiteral("关闭程序"));
	connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

	publishAction = new QAction(QIcon(":/images/publish.png"), QStringLiteral("发布文件"), this);
	publishAction->setStatusTip(QStringLiteral("发布文件"));
	connect(publishAction, SIGNAL(triggered()), this, SLOT(publishFile()));

	clearAction = new QAction(QIcon(":/images/clear.png"), QStringLiteral("清空文本"), this);
	clearAction->setStatusTip(QStringLiteral("清空文本"));
	connect(clearAction, SIGNAL(triggered()), this, SLOT(clearTextEdit()));
}

void PublishFileFrame::createConnectes()
{

}

void PublishFileFrame::createMenus()
{
	fileMenu = menuBar()->addMenu(QStringLiteral("文件"));
	fileMenu->addAction(closeAction);

	operMenu = menuBar()->addMenu(QStringLiteral("操作"));
	operMenu->addAction(publishAction);
	operMenu->addAction(clearAction);
}

void PublishFileFrame::createToolBars()
{
	fileToolBar = addToolBar(QStringLiteral("文件"));
	fileToolBar->addAction(closeAction);

	operToolBar = addToolBar(QStringLiteral("操作"));
	operToolBar->addAction(publishAction);
	operToolBar->addAction(clearAction);
}

void PublishFileFrame::createStatusBar()
{
	statusBar()->showMessage(QStringLiteral("准备就绪"));
}

void PublishFileFrame::updateActions()
{

}

void PublishFileFrame::publishFile()
{
	m_publishing = !m_publishing;

	// 开始发布文件
	setWindowTitle(QStringLiteral("%1: %2").arg(TITLE).arg(QStringLiteral("正在发布文件...")));
	publishAction->setText(QStringLiteral("取消发布文件"));

	m_transFileServer->publishFile(true);
	setWindowTitle(QStringLiteral("%1").arg(TITLE));
}

void PublishFileFrame::clearTextEdit()
{
	if (textEdit)
	{
		textEdit->clear();
	}
}

void PublishFileFrame::updateTextEdit( QString text )
{
	if (textEdit)
	{
		textEdit->moveCursor(QTextCursor::End);
		textEdit->insertPlainText(text);
		textEdit->insertPlainText("\n");
		textEdit->moveCursor(QTextCursor::End);
	}
}
