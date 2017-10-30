#include "publishfileframe.h"
#include "FileHandler.h"

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

	readFileAction = new QAction(QIcon(":/images/read.png"), QStringLiteral("读取文件"), this);
	readFileAction->setStatusTip(QStringLiteral("读取文件"));
	connect(readFileAction, SIGNAL(triggered()), this, SLOT(readFile()));

	writeFileAction = new QAction(QIcon(":/images/write.png"), QStringLiteral("写入文件"), this);
	writeFileAction->setStatusTip(QStringLiteral("写入文件"));
	connect(writeFileAction, SIGNAL(triggered()), this, SLOT(writeFile()));

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
	operMenu->addAction(readFileAction);
	operMenu->addAction(writeFileAction);
	operMenu->addAction(clearAction);
}

void PublishFileFrame::createToolBars()
{
	fileToolBar = addToolBar(QStringLiteral("文件"));
	fileToolBar->addAction(closeAction);

	operToolBar = addToolBar(QStringLiteral("操作"));
	operToolBar->addAction(publishAction);
	operToolBar->addAction(readFileAction);
	operToolBar->addAction(writeFileAction);
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

void PublishFileFrame::readFile()
{
	CFileHandler fileHandler;
	string content;
	bool ok;
	int value = QInputDialog::getInt(this, QStringLiteral("选择读取文件的方式"), 
		QStringLiteral("1: default	2: fstream	3: QFile	4: QTextStream	5: QDataStream"), 
		1, 1, 5, 1, &ok);
	if (ok)
	{
		bool (CFileHandler::*pfun)(const string&, string&);
		QString mode;
		switch(value)
		{
		case 1:
			pfun = &CFileHandler::readFile;
			mode = "readFile";
			break;
		case 2:
			pfun = &CFileHandler::readFileWithFStream;
			mode = "readFileWithFStream";
			break;
		case 3:
			pfun = &CFileHandler::readFileWithQFile;
			mode = "readFileWithQFile";
			break;
		case 4:
			pfun = &CFileHandler::readFileWithQTextStream;
			mode = "readFileWithQTextStream";
			break;
		case 5:
			pfun = &CFileHandler::readFileWithQDataStream;
			mode = "readFileWithQDataStream";
			break;
		default:
			pfun = 0;
			break;
		}

		updateTextEdit(QStringLiteral("以 %1 方式读取文件").arg(mode));
		if ((fileHandler.*pfun)("input.log", content))
		{
			m_fileContent = content;
			updateTextEdit(QString().fromStdString(content));
		}
		else
		{
			updateTextEdit(QStringLiteral("读取文件失败"));
		}
	}
}

void PublishFileFrame::writeFile()
{
	CFileHandler fileHandler;
	string content = m_fileContent;
	if (fileHandler.writeFile("output.log", content))
	{
		updateTextEdit(QStringLiteral("写入文件成功"));
	}
	else
	{
		updateTextEdit(QStringLiteral("写入文件失败"));
	}
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
