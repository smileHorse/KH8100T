#include "restoredb.h"
#include "FastdbManager.h"

#include <QtWidgets/QtWidgets>

RestoreDb::RestoreDb(QWidget *parent)
	: QMainWindow(parent)
{
	m_fastdbManager = FastdbManagerInstance::getFastdbManagerInstance();

	createWidgets();
	createActions();
	createConnects();
	createMenus();
	createToolBars();
	createStatusBar();
}

RestoreDb::~RestoreDb()
{
	FastdbManagerInstance::releaseFastdbManagerInstance();
}

void RestoreDb::createWidgets()
{
	textEdit = new QTextEdit();
	setCentralWidget(textEdit);

	setWindowTitle(QStringLiteral("恢复实时库"));
	setWindowIcon(QIcon(":/images/app.png"));
	resize(800, 600);
}

void RestoreDb::createActions()
{
	openAction = new QAction(QIcon(":/images/open.png"), QStringLiteral("打开实时库"), this);
	openAction->setShortcut(QKeySequence::Open);
	openAction->setStatusTip(QStringLiteral("打开实时库"));
	connect(openAction, SIGNAL(triggered()), this, SLOT(openDb()));

	closeAction = new QAction(QIcon(":/images/close.png"), QStringLiteral("退出程序"), this);
	closeAction->setShortcut(QKeySequence::Close);
	closeAction->setStatusTip(QStringLiteral("退出程序"));
	connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

	restoreAction = new QAction(QIcon(":/images/restore.png"), QStringLiteral("恢复实时库"), this);
	restoreAction->setStatusTip(QStringLiteral("恢复实时库"));
	connect(restoreAction, SIGNAL(triggered()), this, SLOT(restoreDb()));

	clearAction = new QAction(QIcon(":/images/clear.png"), QStringLiteral("清空文本"), this);
	clearAction->setStatusTip(QStringLiteral("清空文本"));
	connect(clearAction, SIGNAL(triggered()), this, SLOT(clearTextEdit()));
}

void RestoreDb::createConnects()
{
	connect(m_fastdbManager, SIGNAL(outputOperationInfo(QString)), this, SLOT(outputOperationInfo(QString)));
}

void RestoreDb::createMenus()
{
	fileMenu = menuBar()->addMenu(QStringLiteral("文件"));
	fileMenu->addAction(openAction);
	fileMenu->addAction(closeAction);

	operMenu = menuBar()->addMenu(QStringLiteral("操作"));
	operMenu->addAction(restoreAction);
	operMenu->addAction(clearAction);
}

void RestoreDb::createToolBars()
{
	fileToolbar = addToolBar(QStringLiteral("文件"));
	fileToolbar->addAction(openAction);
	fileToolbar->addAction(closeAction);

	operToolbar = addToolBar(QStringLiteral("操作"));
	operToolbar->addAction(restoreAction);
	operToolbar->addAction(clearAction);
}

void RestoreDb::createStatusBar()
{
	statusBar()->showMessage(QStringLiteral("准备就绪"));
}

void RestoreDb::openDb()
{
	QString dbName = QFileDialog::getOpenFileName(this, QStringLiteral("选择实时库"));
	if (dbName.isEmpty())
	{
		return;
	}

	if (!m_fastdbManager)
	{
		return;
	}

	dbName = dbName.left(dbName.indexOf("."));
	m_fastdbManager->openDatabase(dbName.toStdString());
}

void RestoreDb::restoreDb()
{
	QString fileName = QFileDialog::getOpenFileName(this, QStringLiteral("选择恢复文件"));
	if (fileName.isEmpty())
	{
		return;
	}

	if (!m_fastdbManager)
	{
		return;
	}

	m_fastdbManager->restoreDatabase(fileName.toStdString());
}

void RestoreDb::clearTextEdit()
{
	if (textEdit)
	{
		textEdit->clear();
	}
}

void RestoreDb::outputOperationInfo( QString text )
{
	if (textEdit)
	{
		textEdit->moveCursor(QTextCursor::End);
		textEdit->insertPlainText(text);
		textEdit->insertPlainText("\n");
		textEdit->moveCursor(QTextCursor::End);
	}
}
