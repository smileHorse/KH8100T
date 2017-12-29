
#include <QtWidgets/QtWidgets>
#include <string>

#include "BaseIceStorm.h"
#include "transferfileclient.h"
#include "transferFileNewI.h"

using namespace std;

TransferFileClient::TransferFileClient(QWidget *parent)
	: QMainWindow(parent), transferMode(Transfer_Ice), fileOperMode(FileOper_Text), fileOperInterface(FileInter_C), 
	adapterIdentify(clientAdapterIdentify), 
	m_communicatorPtr(0), m_objectAdapterPtr(0), 
	m_transferFilePrx(0), m_transferFilePublisher(0)
{
	createWidgets();
	createActions();
	createMenus();
	createToolBars();
	createStatusBar();

	updateUi();
}

TransferFileClient::~TransferFileClient()
{

}

void TransferFileClient::createWidgets()
{
	textEdit = new QTextEdit;
	textEdit->setReadOnly(true);
	setCentralWidget(textEdit);

	setWindowIcon(QIcon(":/icon.png"));
	setWindowTitle(QStringLiteral("传输文件--客户端"));
	resize(800, 600);
}

void TransferFileClient::createActions()
{
	iceAction = createActionImpl(QIcon(":/ice.png"), QStringLiteral("Ice"), 
		QStringLiteral("通过Ice传输文件"), SLOT(useIce()));
	iceStormAction = createActionImpl(QIcon(":/iceStorm.png"), QStringLiteral("IceStorm"), 
		QStringLiteral("通过IceStorm传输文件"), SLOT(useIceStorm()));

	textAction = createActionImpl(QIcon(":/text.png"), QStringLiteral("文本文件"), 
		QStringLiteral("传输文本文件"), SLOT(useText()));
	binaryAction = createActionImpl(QIcon(":/binary.png"), QStringLiteral("二进制文件"), 
		QStringLiteral("传输二进制文件"), SLOT(useBinary()));

	useCAction = createActionImpl(QIcon(":/c.png"), QStringLiteral("标准C接口"), 
		QStringLiteral("使用C接口操作文件"), SLOT(useC()));
	useCPlusPlusAction = createActionImpl(QIcon(":/cPlusPlus.png"), QStringLiteral("标准C++接口"), 
		QStringLiteral("使用C++接口操作文件"), SLOT(useCPlusPlus()));

	selectFileAction = createActionImpl(QIcon(":/selectFile.png"), QStringLiteral("选择文件"), 
		QStringLiteral("选择要请求的文件"), SLOT(selectFile()));
	clearTextEditAction = createActionImpl(QIcon(":/clear.png"), QStringLiteral("清空文本框"), 
		QStringLiteral("清空文本框"), SLOT(clearTextEdit()));
	closeAction = createActionImpl(QIcon(":/close.png"), QStringLiteral("退出程序"), 
		QStringLiteral("退出程序"), SLOT(close()));

	configIceAction = createActionImpl(QIcon(":/config.png"), QStringLiteral("配置Ice服务"), 
		QStringLiteral("配置Ice服务"), SLOT(configIceServer()));
	startIceAction = createActionImpl(QIcon(":/start.png"), QStringLiteral("启动Ice服务"), 
		QStringLiteral("启动Ice服务"), SLOT(startIceServer()));
	stopIceAction = createActionImpl(QIcon(":/stop.png"), QStringLiteral("停止Ice服务"), 
		QStringLiteral("停止Ice服务"), SLOT(stopIceServer()));
}

QAction* TransferFileClient::createActionImpl( const QIcon& icon, const QString& text, const QString& statusTip, 
	const char* slot)
{
	QAction* action = new QAction(icon, text, this);
	action->setStatusTip(statusTip);
	connect(action, SIGNAL(triggered()), this, slot);
	return action;
}

void TransferFileClient::createMenus()
{
	QMenu* transferModeMenu = menuBar()->addMenu(QStringLiteral("文件传输方式"));
	transferModeMenu->addAction(iceAction);
	transferModeMenu->addAction(iceStormAction);

	QMenu* fileModeMenu = menuBar()->addMenu(QStringLiteral("文件读写格式"));
	fileModeMenu->addAction(textAction);
	fileModeMenu->addAction(binaryAction);

	QMenu* fileInterfaceMenu = menuBar()->addMenu(QStringLiteral("文件操作接口"));
	fileInterfaceMenu->addAction(useCAction);
	fileInterfaceMenu->addAction(useCPlusPlusAction);

	QMenu* operMenu = menuBar()->addMenu(QStringLiteral("操作"));
	operMenu->addAction(selectFileAction);
	operMenu->addAction(clearTextEditAction);
	operMenu->addAction(closeAction);

	QMenu* iceMenu = menuBar()->addMenu(QStringLiteral("Ice服务"));
	iceMenu->addAction(configIceAction);
	iceMenu->addAction(startIceAction);
	iceMenu->addAction(stopIceAction);
}

void TransferFileClient::createToolBars()
{
	QToolBar* transferModeBar = addToolBar(QStringLiteral("文件传输方式"));
	transferModeBar->addAction(iceAction);
	transferModeBar->addAction(iceStormAction);

	QToolBar* fileModeModeBar = addToolBar(QStringLiteral("文件读写格式"));
	fileModeModeBar->addAction(textAction);
	fileModeModeBar->addAction(binaryAction);

	QToolBar* fileInterfaceBar = addToolBar(QStringLiteral("文件操作接口"));
	fileInterfaceBar->addAction(useCAction);
	fileInterfaceBar->addAction(useCPlusPlusAction);

	QToolBar* operBar = addToolBar(QStringLiteral("操作"));
	operBar->addAction(selectFileAction);
	operBar->addAction(clearTextEditAction);
	operBar->addAction(closeAction);

	QToolBar* iceBar = addToolBar(QStringLiteral("Ice服务"));
	iceBar->addAction(configIceAction);
	iceBar->addAction(startIceAction);
	iceBar->addAction(stopIceAction);
}

void TransferFileClient::createStatusBar()
{
	statusBar()->showMessage(QStringLiteral("准备就绪"));
}

void TransferFileClient::updateUi()
{
	iceAction->setEnabled(transferMode != Transfer_Ice);
	iceStormAction->setEnabled(transferMode != Transfer_IceStorm);

	textAction->setEnabled(fileOperMode != FileOper_Text);
	binaryAction->setEnabled(fileOperMode != FileOper_Binary);

	useCAction->setEnabled(fileOperInterface != FileInter_C);
	useCPlusPlusAction->setEnabled(fileOperInterface != FileInter_CPlusPlus);
}

void TransferFileClient::transferFile( const QString& filePath )
{
	if (getFileOperInterface() == FileInter_C)
	{
		transferFile_C(filePath);
	}
	else if (getFileOperInterface() == FileInter_CPlusPlus)
	{
		transferFile_CPlusPlus(filePath);
	}
}

void TransferFileClient::transferFile_C( const QString& filePath )
{
}

void TransferFileClient::transferFile_CPlusPlus( const QString& filePath )
{

}

void TransferFileClient::useIce()
{
	transferMode = Transfer_Ice;
	updateUi();
}

void TransferFileClient::useIceStorm()
{
	transferMode = Transfer_IceStorm;
	updateUi();
}

void TransferFileClient::useText()
{
	fileOperMode = FileOper_Text;
	updateUi();
}

void TransferFileClient::useBinary()
{
	fileOperMode = FileOper_Binary;
	updateUi();
}

void TransferFileClient::useC()
{
	fileOperInterface = FileInter_C;
	updateUi();
}

void TransferFileClient::useCPlusPlus()
{
	fileOperInterface = FileInter_CPlusPlus;
	updateUi();
}

void TransferFileClient::selectFile()
{
	QString filePath = QFileDialog::getOpenFileName(this, QStringLiteral("选择文件"));
	if (filePath.isEmpty())
	{
		return;
	}

	// 发送文件
	filePath.replace("\\", "/");
	transferFile(filePath);
}

void TransferFileClient::clearTextEdit()
{
	if (textEdit)
	{
		textEdit->clear();
	}
}

void TransferFileClient::configIceServer()
{
	ConfigIceDialog dialog;
	if (dialog.exec() == QDialog::Accepted)
	{
		configIceInfo = dialog.getConfigIceInfo();
		BaseIceStorm::SetIceStormIpAndPort(configIceInfo.iceStormIp.toStdString(), configIceInfo.iceStormPort);
	}
}

void TransferFileClient::startIceServer()
{
	if (m_communicatorPtr)
	{
		return;
	}

	try
	{
		int argc = 0;
		Ice::InitializationData initData;
		initData.properties = Ice::createProperties();
		m_communicatorPtr = Ice::initialize(argc, 0, initData);
		QString endPoints = QString("tcp -h %1 -p %2").arg(configIceInfo.iceIp).arg(configIceInfo.icePort);
		m_objectAdapterPtr = m_communicatorPtr->createObjectAdapterWithEndpoints(adapterIdentify.toStdString(), 
			endPoints.toStdString());

		m_objectAdapterPtr->add(new TransferFileI(this), 
			m_communicatorPtr->stringToIdentity(clientProxyIdentify.toStdString()));

		m_objectAdapterPtr->activate();

		updateTextEdit(QStringLiteral("客户端 -- Ice服务启动成功"));
	}
	catch(const Ice::Exception& ex)
	{
		QMessageBox::warning(this, QStringLiteral("启动Ice服务异常"), ex.what());
	}
}

void TransferFileClient::stopIceServer()
{
	if (m_communicatorPtr)
	{
		try
		{
			m_communicatorPtr->destroy();
			
			m_communicatorPtr = 0;
			m_objectAdapterPtr = 0;

			updateTextEdit(QStringLiteral("客户端 -- Ice服务停止成功"));
		}
		catch(const Ice::Exception& ex)
		{
			QMessageBox::warning(this, QStringLiteral("停止Ice服务异常"), ex.what());
		}
	}
}

void TransferFileClient::updateTextEdit( const QString& text )
{
	if (textEdit)
	{
		textEdit->moveCursor(QTextCursor::End);
		textEdit->insertPlainText(text);
		textEdit->insertPlainText("\n");
		textEdit->moveCursor(QTextCursor::End);
	}
}

TransferMode TransferFileClient::getTransferMode() const
{
	return transferMode;
}

FileOperMode TransferFileClient::getFileOperMode() const
{
	return fileOperMode;
}

FileOperInterface TransferFileClient::getFileOperInterface() const
{
	return fileOperInterface;
}

TransferFileSpace::TransferFilePrx TransferFileClient::getTransferFilePrx()
{
	if (m_transferFilePrx == 0)
	{
		try
		{
			QString proxy = QString("%1: tcp -h %2 -p %3").arg(serverAdapterIdentify).
				arg(configIceInfo.iceOffsideIp).arg(configIceInfo.iceOffsidePort);
			m_transferFilePrx = TransferFileSpace::TransferFilePrx::checkedCast(
				m_communicatorPtr->stringToProxy(proxy.toStdString()));
			if (m_transferFilePrx != 0)
			{
				updateTextEdit(QStringLiteral("创建传输文件的Ice代理成功"));
			}
			else
			{
				updateTextEdit(QStringLiteral("创建传输文件的Ice代理失败"));
			}
		}
		catch(const Ice::Exception& ex)
		{
			updateTextEdit(QStringLiteral("创建传输文件的Ice代理异常: %1").arg(ex.what()));
			m_transferFilePrx = 0;
		}
	}
	
	return m_transferFilePrx;
}

TransferFileSpace::TransferFilePrx TransferFileClient::getTransferFilePublisher()
{
	if (m_transferFilePublisher == 0)
	{
		string topic = TransferFileSpace::TransferFileTopic;
		string deliverModel = "oneway";
		try
		{
			Ice::ObjectPrx objectPrx = BaseIceStorm::GetPublisher(m_communicatorPtr, topic, deliverModel);
			if (objectPrx != 0)
			{
				m_transferFilePublisher = TransferFileSpace::TransferFilePrx::uncheckedCast(objectPrx);
				updateTextEdit(QStringLiteral("创建传输文件的IceStorm发布代理成功"));
			}
			else
			{
				m_transferFilePublisher = 0;
				updateTextEdit(QStringLiteral("创建传输文件的IceStorm发布代理失败"));
			}
		}
		catch(const Ice::Exception& ex)
		{
			updateTextEdit(QStringLiteral("创建传输文件的IceStorm发布代理异常: %1").arg(ex.what()));
			m_transferFilePublisher = 0;
		}
	}
	
	return m_transferFilePublisher;
}
