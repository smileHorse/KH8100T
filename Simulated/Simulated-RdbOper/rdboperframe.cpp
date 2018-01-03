
#include <QtWidgets/QtWidgets>

#include "rdboperframe.h"
#include "rdbTableModel.h"

RdbOperFrame::RdbOperFrame(QWidget *parent)
	: QMainWindow(parent), m_adapterIdentify("SimulatedRdbOpt")
{
	createWidget();
	createActions();
	createMenus();
	createToolBars();
	createStatusBar();

	initIcePtr();
}

RdbOperFrame::~RdbOperFrame()
{

}

void RdbOperFrame::createWidget()
{
	listView = new QListView;
	listView->setModel(new RdbTableNameModel);

	tableView = new QTableView;

	QHBoxLayout* layout = new QHBoxLayout;
	layout->addWidget(listView, 1);
	layout->addWidget(tableView, 4);
	
	QWidget* widget = new QWidget;
	widget->setLayout(layout);
	setCentralWidget(widget);

	setWindowTitle(QStringLiteral("实时库操作"));
	setWindowIcon(QIcon(":/icon.png"));
	resize(800, 600);
}

void RdbOperFrame::createActions()
{
	quitAppAction = createActionImpl(QIcon(":/quit.png"), QStringLiteral("退出程序"), 
		QStringLiteral("退出程序"), SLOT(close()));

	configServerAction = createActionImpl(QIcon(":/configServer.png"), QStringLiteral("配置Ice服务"), 
			QStringLiteral("配置Ice服务"), SLOT(configServer()));
	startServerAction = createActionImpl(QIcon(":/startServer.png"), QStringLiteral("启动Ice服务"), 
		QStringLiteral("启动Ice服务"), SLOT(startServer()));
	stopServerAction = createActionImpl(QIcon(":/stopServer.png"), QStringLiteral("停止Ice服务"), 
		QStringLiteral("停止Ice服务"), SLOT(stopServer()));

	selectDefaultDataAction = createActionImpl(QIcon(":/select.png"), QStringLiteral("查询默认数据"), 
		QStringLiteral("查询默认数据"), SLOT(selectDefaultData())); 
	selectSpecficDataAction = createActionImpl(QIcon(":/select.png"), QStringLiteral("查询指定数据"), 
		QStringLiteral("查询指定数据"), SLOT(selectSpecficData())); 
	selectCompleteDataAction = createActionImpl(QIcon(":/select.png"), QStringLiteral("查询全部数据"), 
		QStringLiteral("查询全部数据"), SLOT(selectCompleteData())); 
}

QAction* RdbOperFrame::createActionImpl(const QIcon& icon, const QString& text, const QString& statusTip, const char* slot)
{
	QAction* action = new QAction(icon, text, this);
	action->setStatusTip(statusTip);
	connect(action, SIGNAL(triggered()), this, slot);

	return action;
}

void RdbOperFrame::createMenus()
{
	QMenu* fileMenu = menuBar()->addMenu(QStringLiteral("文件"));
	fileMenu->addAction(quitAppAction);

	QMenu* serverMenu = menuBar()->addMenu(QStringLiteral("Ice服务"));
	serverMenu->addAction(configServerAction);
	serverMenu->addAction(startServerAction);
	serverMenu->addAction(stopServerAction);

	QMenu* operMenu = menuBar()->addMenu(QStringLiteral("操作"));
	operMenu->addAction(selectDefaultDataAction);
	operMenu->addAction(selectSpecficDataAction);
	operMenu->addAction(selectCompleteDataAction);
}

void RdbOperFrame::createToolBars()
{
	QToolBar* fileToolBar = addToolBar(QStringLiteral("文件"));
	fileToolBar->addAction(quitAppAction);

	QToolBar* serverToolBar = addToolBar(QStringLiteral("Ice服务"));
	serverToolBar->addAction(configServerAction);
	serverToolBar->addAction(startServerAction);
	serverToolBar->addAction(stopServerAction);

	QToolBar* operToolBar = addToolBar(QStringLiteral("操作"));
	operToolBar->addAction(selectDefaultDataAction);
	operToolBar->addAction(selectSpecficDataAction);
	operToolBar->addAction(selectCompleteDataAction);
}

void RdbOperFrame::createStatusBar()
{
	statusBar()->showMessage(QStringLiteral("准备就绪"));
}

void RdbOperFrame::initIcePtr()
{
	m_communicatorPtr = 0;
	m_objectAdapterPtr = 0;
	m_rdbDataOptPrx = 0;
}

bool RdbOperFrame::getRdbDataOptPrx()
{
	if (m_rdbDataOptPrx)
	{
		return true;
	}

	try
	{
		QString proxy = QString("rdb-opt: tcp -h %1 -p %2").arg(m_configIceInfo.iceOffsideIp).
				arg(m_configIceInfo.iceOffsidePort);
		m_rdbDataOptPrx = RdbDataOptPrx::checkedCast(m_communicatorPtr->stringToProxy(proxy.toStdString()));
		
		return true;
	}
	catch(const Ice::Exception& ex)
	{
		QMessageBox::warning(this, QStringLiteral("获取实时库数据代理"), 
			QStringLiteral("获取代理失败: %1").arg(ex.what()));
		m_rdbDataOptPrx = 0;
		return false;
	}
}

void RdbOperFrame::configServer()
{
	ConfigIceDialog configDialog(QIcon(":/configServer.png"));
	if (configDialog.exec() == QDialog::Accepted)
	{
		m_configIceInfo = configDialog.getConfigIceInfo();
	}
}

void RdbOperFrame::startServer()
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
		QString endPoints = QString("tcp -h %1 -p %2").arg(m_configIceInfo.iceIp).arg(m_configIceInfo.icePort);
		m_objectAdapterPtr = m_communicatorPtr->createObjectAdapterWithEndpoints(m_adapterIdentify.toStdString(), 
			endPoints.toStdString());

		m_objectAdapterPtr->activate();

		updateStatusBar(QStringLiteral("Ice服务启动成功"));
	}
	catch(const Ice::Exception& ex)
	{
		initIcePtr();
		QMessageBox::warning(this, QStringLiteral("启动Ice服务"), 
			QStringLiteral("启动服务失败: %1").arg(ex.what()));
	}
}

void RdbOperFrame::stopServer()
{
	if (m_communicatorPtr)
	{
		try
		{
			m_communicatorPtr->destroy();

			initIcePtr();
			updateStatusBar(QStringLiteral("Ice服务停止成功"));
		}
		catch(const Ice::Exception& ex)
		{
			initIcePtr();
			QMessageBox::warning(this, QStringLiteral("停止Ice服务"), 
				QStringLiteral("停止服务失败: %1").arg(ex.what()));
		}
	}
}

void RdbOperFrame::selectDefaultData()
{
	if (!getRdbDataOptPrx())
	{
		return;
	}

	try
	{
		RequestDefaultDataSeq reqSeq;
		reqSeq.id = 1;
		reqSeq.requestId = 1;
		reqSeq.requestNode = "localhost";
		reqSeq.isStop = true;
		reqSeq.refreshFreq = 0;
		
		RequestDefaultData reqData;
		reqData.tableName = "Area";
		reqData.fieldName = "name";
		reqData.fieldValue = "111";

		reqSeq.seq.push_back(reqData);
		reqSeq.dataCount = reqSeq.seq.size();

		RespondDefaultDataSeq repSeq;
		m_rdbDataOptPrx->SelectDefaultData(reqSeq, repSeq);
	}
	catch(const Ice::Exception& ex)
	{
		QMessageBox::warning(this, QStringLiteral("查询默认数据"), 
			QStringLiteral("查询数据失败: %1").arg(ex.what()));
	}
}

void RdbOperFrame::selectSpecficData()
{
	if (!getRdbDataOptPrx())
	{
		return;
	}

	try
	{
		//m_rdbDataOptPrx->SelectSpecificData(reqSeq, repSeq);
	}
	catch(const Ice::Exception& ex)
	{
		QMessageBox::warning(this, QStringLiteral("查询指定数据"), 
			QStringLiteral("查询数据失败: %1").arg(ex.what()));
	}
}

void RdbOperFrame::selectCompleteData()
{
	if (!getRdbDataOptPrx())
	{
		return;
	}

	try
	{
		//m_rdbDataOptPrx->SelectCompleteData(reqSeq, repSeq);
	}
	catch(const Ice::Exception& ex)
	{
		QMessageBox::warning(this, QStringLiteral("查询全部数据"), 
			QStringLiteral("查询数据失败: %1").arg(ex.what()));
	}
}

void RdbOperFrame::updateTableView()
{

}

void RdbOperFrame::updateStatusBar( const QString& text, int timeout )
{
	statusBar()->showMessage(text, timeout);
}
