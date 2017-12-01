
#include "fileTransferFrame.h"

#include <QtWidgets/QtWidgets>
#include <QtCore/QtCore>

FileTransferFrame::FileTransferFrame(int argc, char* argv[], QWidget* parent /*= 0*/ )
	: QMainWindow(parent), m_serverStarted(false)
{
	createWidgets();
	createActions();
	createConnections();
	createMenus();
	createToolBars();
	createStatusBar();

	m_iceServer.setParam(argc, argv);
}

void FileTransferFrame::createWidgets()
{
	tableWidget = new QTableWidget;
	tableWidget->setShowGrid(true);
	tableWidget->setAlternatingRowColors(true);
	QStringList labels;
	labels << QStringLiteral("操作模式") << QStringLiteral("文件名称") << QStringLiteral("文件大小") 
		<< QStringLiteral("传输方式") << QStringLiteral("传输时间");
	tableWidget->setColumnCount(labels.size());
	tableWidget->setHorizontalHeaderLabels(labels);
	setCentralWidget(tableWidget);

	progressBar = new QProgressBar;
	progressBar->setRange(0, 100);

	resize(1000, 800);
	setWindowTitle(QStringLiteral("文件传输"));
	setWindowIcon(QIcon(":/app.png"));
}

void FileTransferFrame::createActions()
{
	quitAction = new QAction(QIcon(":/quit.png"), QStringLiteral("退出程序"), this);
	quitAction->setShortcuts(QKeySequence::Quit);

	configLocalAction = new QAction(QIcon(":/configLocal.png"), QStringLiteral("配置本地主机信息"), this);
	startClientAction = new QAction(QIcon(":/start.png"), QStringLiteral("启动本地服务"), this);
	startClientAction->setEnabled(false);
	stopClientAction = new QAction(QIcon(":/stop.png"), QStringLiteral("停止本地服务"), this);
	stopClientAction->setEnabled(false);

	configRemoteAction = new QAction(QIcon(":/configRemote.png"), QStringLiteral("配置远方主机信息"), this);
	uploadFileAction = new QAction(QIcon(":/upload.png"), QStringLiteral("上传文件"), this);
	uploadFileAction->setEnabled(false);
	downloadFileAction = new QAction(QIcon(":/download.png"), QStringLiteral("下载文件"), this);
	downloadFileAction->setEnabled(false);

	clearAction = new QAction(QIcon(":/clear.png"), QStringLiteral("清空列表"), this);
}

void FileTransferFrame::createConnections()
{
	connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));

	connect(configLocalAction, SIGNAL(triggered()), this, SLOT(configLocal()));
	connect(startClientAction, SIGNAL(triggered()), this, SLOT(startLocalServer()));
	connect(stopClientAction, SIGNAL(triggered()), this, SLOT(stopLocalServer()));

	connect(configRemoteAction, SIGNAL(triggered()), this, SLOT(configRemote()));
	connect(uploadFileAction, SIGNAL(triggered()), this, SLOT(uploadFile()));
	connect(downloadFileAction, SIGNAL(triggered()), this, SLOT(downloadFile()));

	connect(clearAction, SIGNAL(triggered()), this, SLOT(clearTabelWidget()));

	connect(&m_iceServer, SIGNAL(serverStarted(bool, const QString&)), this, SLOT(serverStarted(bool, const QString&)));
	connect(&m_iceServer, SIGNAL(emitMessage(const QString&)), this, SLOT(outputMessage(const QString&)));
	connect(&m_iceServer, SIGNAL(emitTransferFileItem(const TransferFileItem&)), 
			this, SLOT(updateTableWidget(const TransferFileItem&)));
	connect(&m_iceServer, SIGNAL(emitProgress(int)), this, SLOT(updateProgress(int)));
}

void FileTransferFrame::createMenus()
{
	QMenu* fileMenu = menuBar()->addMenu(QStringLiteral("文件"));
	fileMenu->addAction(quitAction);

	QMenu* localMenu = menuBar()->addMenu(QStringLiteral("本地服务"));
	localMenu->addAction(configLocalAction);
	localMenu->addAction(startClientAction);
	localMenu->addAction(stopClientAction);

	QMenu* remoteMenu = menuBar()->addMenu(QStringLiteral("远方服务"));
	remoteMenu->addAction(configRemoteAction);
	remoteMenu->addAction(uploadFileAction);
	remoteMenu->addAction(downloadFileAction);

	QMenu* operMenu = menuBar()->addMenu(QStringLiteral("操作"));
	operMenu->addAction(clearAction);
}

void FileTransferFrame::createToolBars()
{
	QToolBar* fileToolBar = addToolBar(QStringLiteral("文件"));
	fileToolBar->addAction(quitAction);

	QToolBar* localToolBar = addToolBar(QStringLiteral("本地服务"));
	localToolBar->addAction(configLocalAction);
	localToolBar->addAction(startClientAction);
	localToolBar->addAction(stopClientAction);

	QToolBar* remoteToolBar = addToolBar(QStringLiteral("远方服务"));
	remoteToolBar->addAction(configRemoteAction);
	remoteToolBar->addAction(uploadFileAction);
	remoteToolBar->addAction(downloadFileAction);

	QToolBar* operToolBar = addToolBar(QStringLiteral("操作"));
	operToolBar->addAction(clearAction);
}

void FileTransferFrame::createStatusBar()
{
	statusBar()->showMessage(QStringLiteral("准备就绪"));
	statusBar()->addWidget(progressBar);
}

void FileTransferFrame::updateUi()
{
	if (m_serverStarted)
	{
		startClientAction->setEnabled(false);
		stopClientAction->setEnabled(true);
	}
	else
	{
		startClientAction->setEnabled(true);
		stopClientAction->setEnabled(false);

		uploadFileAction->setEnabled(false);
		downloadFileAction->setEnabled(false);
	}
}

void FileTransferFrame::configLocal()
{
	QString title = QStringLiteral("配置本地主机信息");
	QString label = QStringLiteral("本地主机Ip: ");
	QString host = QInputDialog::getText(this, title, label, QLineEdit::Normal, "192.168.3.25");
	if (!host.isEmpty())
	{
		label = QStringLiteral("本地主机Port: ");
		int port = QInputDialog::getInt(this, title, label, 11000);

		m_iceServer.setLocalHost(host, port);

		startClientAction->setEnabled(true);
	}
}

void FileTransferFrame::configRemote()
{
	QString title = QStringLiteral("配置远方主机信息");
	QString label = QStringLiteral("远方主机Ip: ");
	QString host = QInputDialog::getText(this, title, label, QLineEdit::Normal, "192.168.3.25");
	if (!host.isEmpty())
	{
		label = QStringLiteral("远方主机Port: ");
		int port = QInputDialog::getInt(this, title, label, 11001);

		m_iceServer.setRemoteHostPort(host, port);

		if (m_serverStarted)
		{
			uploadFileAction->setEnabled(true);
			downloadFileAction->setEnabled(true);
		}
	}
}

void FileTransferFrame::startLocalServer()
{
	QThread* thread = new QThread(this);
	m_iceServer.moveToThread(thread);
	connect(thread, SIGNAL(started()), &m_iceServer, SLOT(start()));
	thread->start();

	m_serverStarted = true;
	updateUi();
}

void FileTransferFrame::stopLocalServer()
{
	m_iceServer.shutdown();

	m_serverStarted = false;
	updateUi();
}

void FileTransferFrame::uploadFile()
{
	QString caption = QStringLiteral("选择上传文件");
	QString fileName = QFileDialog::getOpenFileName(this, caption, "./file/upload");
	if (!fileName.isEmpty())
	{
		m_iceServer.uploadFile(fileName);
	}
}

void FileTransferFrame::downloadFile()
{
	QString fileName = QInputDialog::getText(this, QStringLiteral("请输入文件名称"), QStringLiteral("文件名称:"));
	if (!fileName.isEmpty())
	{
		m_iceServer.downloadFile(fileName);
	}
}

void FileTransferFrame::outputMessage( const QString& msg )
{
	QMessageBox::warning(this, QStringLiteral("告警"), msg);
}

void FileTransferFrame::updateStatusBar( const QString& msg )
{
	statusBar()->showMessage(msg);
}

void FileTransferFrame::updateTableWidget(const TransferFileItem& item )
{
	int rowCount = tableWidget->rowCount();
	tableWidget->insertRow(rowCount);
	if (item.operMode == UploadFile)
	{
		tableWidget->setItem(rowCount, 0, new QTableWidgetItem(QIcon(":/upload.png"), QStringLiteral("上传")));
	}
	else
	{
		tableWidget->setItem(rowCount, 0, new QTableWidgetItem(QIcon(":/download.png"), QStringLiteral("下载")));
	}
	tableWidget->setItem(rowCount, 1, new QTableWidgetItem(item.fileName));
	tableWidget->setItem(rowCount, 2, new QTableWidgetItem(item.fileSize));
	tableWidget->setItem(rowCount, 3, new QTableWidgetItem(item.transMode));
	tableWidget->setItem(rowCount, 4, new QTableWidgetItem(item.transTime));

	tableWidget->resizeColumnsToContents();
}

void FileTransferFrame::clearTabelWidget()
{
	if (tableWidget)
	{
		tableWidget->clearContents();
	}
}

void FileTransferFrame::serverStarted( bool status, const QString& msg )
{
	m_serverStarted = status;
	updateUi();

	if (!status)
	{
		QMessageBox::warning(this, QStringLiteral("告警"), msg);
	}
}

void FileTransferFrame::updateProgress( int value )
{
	progressBar->setValue(value);
}
