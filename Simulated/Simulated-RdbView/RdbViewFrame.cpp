#include "rdbviewframe.h"
#include "FdbTableDefine.h"
#include "tableoperation.h"
#include "hidecolumndialog.h"

#include <QtWidgets/QtWidgets>

RdbViewFrame::RdbViewFrame(QWidget *parent)
	: QMainWindow(parent)
{
	m_dbPtr = new dbDatabase(dbDatabase::dbAllAccess);
	m_tableOperPtr = new TableOperation(m_dbPtr);

	createWidgets();
	createActions();
	createMenus();
	createToolbars();
	createStatusBar();
	createConnects();
}

RdbViewFrame::~RdbViewFrame()
{

}

void RdbViewFrame::createWidgets()
{
	m_rdbTreeWidget = new QTreeWidget();
	m_rdbTreeWidget->setColumnCount(1);
	m_rdbTreeWidget->setHeaderLabels(QStringList(QStringLiteral("实时库列表")));
	m_rdbTreeWidget->setRootIsDecorated(false);
	m_rdbTreeWidget->setSelectionMode(QAbstractItemView::SingleSelection);

	m_rdbTableWidget = new QTableWidget;
	m_rdbTableWidget->setWindowTitle(QStringLiteral("实时库内容"));
	m_rdbTableWidget->setShowGrid(true);
	m_rdbTableWidget->setSortingEnabled(true);
	m_rdbTableWidget->setAlternatingRowColors(true);

	QSplitter* splitter = new QSplitter;
	splitter->addWidget(m_rdbTreeWidget);
	splitter->addWidget(m_rdbTableWidget);
	splitter->setStretchFactor(0, 1);
	splitter->setStretchFactor(1, 6);

	setCentralWidget(splitter);
	setWindowTitle(QStringLiteral("实时库操作"));
	setWindowIcon(QIcon(":/images/rdbview.png"));
	resize(1400, 800);
}

void RdbViewFrame::createActions()
{
	openAction = new QAction(QIcon(":/images/open.png"), QStringLiteral("打开实时库"), this);
	openAction->setShortcut(QKeySequence::Open);
	openAction->setStatusTip(QStringLiteral("打开本地实时数据库"));
	connect(openAction, SIGNAL(triggered()), this, SLOT(openRdbDatabase()));

	closeAction = new QAction(QIcon(":/images/close.png"), QStringLiteral("关闭实时库"), this);
	closeAction->setShortcut(QKeySequence::Close);
	closeAction->setStatusTip(QStringLiteral("关闭本地实时数据库"));
	closeAction->setEnabled(false);
	connect(closeAction, SIGNAL(triggered()), this, SLOT(closeRdbDatabase()));

	exitAction = new QAction(QIcon(":/images/exit.png"), QStringLiteral("退出"), this);
	exitAction->setShortcut(QKeySequence::Quit);
	exitAction->setStatusTip(QStringLiteral("退出系统"));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

	addAction = new QAction(QIcon(":/images/add.png"), QStringLiteral("新增"), this);
	addAction->setStatusTip(QStringLiteral("新增数据"));
	addAction->setEnabled(false);
	connect(addAction, SIGNAL(triggered()), this, SLOT(addData()));

	deleteAction = new QAction(QIcon(":/images/delete.png"), QStringLiteral("删除"), this);
	deleteAction->setStatusTip(QStringLiteral("删除数据"));
	deleteAction->setEnabled(false);
	connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteData()));

	saveAction = new QAction(QIcon(":/images/save.png"), QStringLiteral("保存"), this);
	saveAction->setStatusTip(QStringLiteral("保存数据"));
	saveAction->setEnabled(false);
	connect(saveAction, SIGNAL(triggered()), this, SLOT(saveData()));

	refreshAction = new QAction(QIcon(":/images/refresh.png"), QStringLiteral("刷新"), this);
	refreshAction->setStatusTip(QStringLiteral("刷新数据"));
	refreshAction->setEnabled(false);
	connect(refreshAction, SIGNAL(triggered()), this, SLOT(refreshData()));

	deleteAllAction = new QAction(QIcon(":/images/deleteAll.png"), QStringLiteral("全部删除"), this);
	deleteAllAction->setStatusTip(QStringLiteral("删除全部数据"));
	deleteAllAction->setEnabled(false);
	connect(deleteAllAction, SIGNAL(triggered()), this, SLOT(deleteAllData()));

	hideTableColumnAction = new QAction(QIcon(":/images/hideColumn.png"), QStringLiteral("隐藏列"), this);
	hideTableColumnAction->setStatusTip(QStringLiteral("隐藏表格中的指定列"));
	hideTableColumnAction->setEnabled(true);
	connect(hideTableColumnAction, SIGNAL(triggered()), this, SLOT(hideTableColumn()));

	aboutAction = new QAction(QIcon(":/images/about.png"), QStringLiteral("关于"), this);
	aboutAction->setStatusTip(QStringLiteral("关于程序"));
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

void RdbViewFrame::createMenus()
{
	fileMenu = menuBar()->addMenu(QStringLiteral("文件"));
	fileMenu->addAction(openAction);
	fileMenu->addAction(closeAction);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);

	editMenu = menuBar()->addMenu(QStringLiteral("编辑"));
	editMenu->addAction(addAction);
	editMenu->addAction(deleteAction);
	editMenu->addAction(saveAction);
	editMenu->addAction(refreshAction);
	editMenu->addSeparator();
	editMenu->addAction(deleteAllAction);
	editMenu->addAction(hideTableColumnAction);

	helpMenu = menuBar()->addMenu(QStringLiteral("帮助"));
	helpMenu->addAction(aboutAction);
}

void RdbViewFrame::createToolbars()
{
	fileToolBar = addToolBar(QStringLiteral("文件"));
	fileToolBar->addAction(openAction);
	fileToolBar->addAction(closeAction);
	fileToolBar->addSeparator();
	fileToolBar->addAction(exitAction);

	editToolBar = addToolBar(QStringLiteral("编辑"));
	editToolBar->addAction(addAction);
	editToolBar->addAction(deleteAction);
	editToolBar->addAction(saveAction);
	editToolBar->addAction(refreshAction);
	editToolBar->addSeparator();
	editToolBar->addAction(deleteAllAction);
	editToolBar->addAction(hideTableColumnAction);
}

void RdbViewFrame::createStatusBar()
{
	statusBar()->showMessage(QStringLiteral("准备就绪"));
}

void RdbViewFrame::createConnects()
{
	connect(this, SIGNAL(databaseOpenState(bool)), this, SLOT(updateDatabaseActions(bool)));
	connect(this, SIGNAL(databaseOpenState(bool)), this, SLOT(updateTreeWidget()));
	connect(m_rdbTreeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, 
		SLOT(updateTableWidget(QTreeWidgetItem*, QTreeWidgetItem*)));
	connect(m_rdbTreeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, 
		SLOT(treeItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
	connect(m_rdbTableWidget,SIGNAL(currentItemChanged(QTableWidgetItem*,QTableWidgetItem*)), this, 
		SLOT(tableItemChanged(QTableWidgetItem*,QTableWidgetItem*)));
}

void RdbViewFrame::refreshDatas( QTreeWidgetItem* item )
{
	// 清空内容表
	QStringList tableHeaders;
	m_rdbTableWidget->clearContents();
	m_rdbTableWidget->setRowCount(0);
	m_rdbTableWidget->setHorizontalHeaderLabels(tableHeaders);
	m_rdbTableWidget->setColumnCount(0);

	if (item)
	{
		QString tableName = item->text(0);
		// 填写表头
		tableHeaders = m_tableOperPtr->getTableFieldNames(tableName);
		m_rdbTableWidget->setColumnCount(tableHeaders.count());
		m_rdbTableWidget->setHorizontalHeaderLabels(tableHeaders);

		// 填写表数据
		QList<QStringList> datas = m_tableOperPtr->selectDatas(tableName);
		deleteAllAction->setEnabled(!datas.isEmpty());
		for (int i = 0; i < datas.count(); ++i)
		{
			int count = m_rdbTableWidget->rowCount();
			m_rdbTableWidget->insertRow(count);
			for (int j = 0; j < datas.at(i).count(); ++j)
			{
				m_rdbTableWidget->setItem(count, j, new QTableWidgetItem(datas.at(i).at(j)));
			}
		}

		// 自动隐藏不重要的字段
		QList<int> hideColumns;
		m_tableOperPtr->getHidedColumns(tableName, hideColumns);
		foreach(int i, hideColumns)
		{
			m_rdbTableWidget->hideColumn(i);
		}
	}
}

bool RdbViewFrame::getCurrentTableName( QString& tableName )
{
	QTreeWidgetItem* item = m_rdbTreeWidget->currentItem();
	if (!item)
	{
		QMessageBox::critical(this, QStringLiteral("实时库操作"), QStringLiteral("尚未选择任何实时库"));
		return false;
	}

	tableName = item->text(0);
	return true;
}

bool RdbViewFrame::getCurrentMRID( QString& mRID )
{
	int row = m_rdbTableWidget->currentRow();
	if (row < 0)
	{
		QMessageBox::critical(this, QStringLiteral("实时库操作"), QStringLiteral("当前行数小于0"));
		return false;
	}

	QTableWidgetItem* item = m_rdbTableWidget->item(row, 0);
	if (!item)
	{
		QMessageBox::critical(this, QStringLiteral("实时库操作"), QStringLiteral("当前选中项无效"));
		return false;
	}

	mRID = item->text();
	return true;
}

bool RdbViewFrame::getCurrentTableItemValue( QString& value )
{
	QTableWidgetItem* item = m_rdbTableWidget->currentItem();
	if (item)
	{
		value = item->text();
		return true;
	}
	else
	{
		QMessageBox::critical(this, QStringLiteral("实时库操作"), QStringLiteral("当前选中项无效"));
		return false;
	}
}

bool RdbViewFrame::getCurrentTableHeaderLabel( QString& value )
{
	QTableWidgetItem* item = m_rdbTableWidget->currentItem();
	if (item)
	{
		int column = item->column();
		QTableWidgetItem* headerItem = m_rdbTableWidget->horizontalHeaderItem(column);
		if (headerItem)
		{
			value = headerItem->text();
		}
		else
		{
			QMessageBox::critical(this, QStringLiteral("实时库操作"), QStringLiteral("当前选中项无效"));
			return false;
		}
	}
	else
	{
		QMessageBox::critical(this, QStringLiteral("实时库操作"), QStringLiteral("当前选中项无效"));
		return false;
	}
	return true;
}

void RdbViewFrame::updateDatabaseActions( bool opened )
{
	openAction->setEnabled(!opened);
	closeAction->setEnabled(opened);
}

void RdbViewFrame::updateTreeWidget()
{
	m_rdbTreeWidget->clear();

	if (m_dbPtr->isOpen())
	{
		QStringList tableNames;
		dbTableDescriptor* tableDesc = m_dbPtr->getTables();
		do 
		{
			if (tableDesc)
			{
				tableNames.push_back(tableDesc->getName());
			}
			else
			{
				break;
			}
		} while (tableDesc = tableDesc->getNextTable());


		for (int i = 0; i < tableNames.count(); ++i) {
			QTreeWidgetItem *item = new QTreeWidgetItem(m_rdbTreeWidget);
			item->setText(0, tableNames[i]);
		}

		if (tableNames.count() > 0)
			m_rdbTreeWidget->setCurrentItem(m_rdbTreeWidget->topLevelItem(0));
	}
}

void RdbViewFrame::updateTableWidget( QTreeWidgetItem* current, QTreeWidgetItem* previous )
{
	if (current == previous)
	{
		return;
	}

	refreshDatas(current);
}

void RdbViewFrame::treeItemChanged( QTreeWidgetItem* current, QTreeWidgetItem* previous )
{
	if (current)
	{
		addAction->setEnabled(true);
		refreshAction->setEnabled(true);
	}
	else
	{
		addAction->setEnabled(false);
		refreshAction->setEnabled(false);
	}
}

void RdbViewFrame::tableItemChanged( QTableWidgetItem* current, QTableWidgetItem* previous )
{
	if (current)
	{
		deleteAction->setEnabled(true);
		saveAction->setEnabled(true);
	}
	else
	{
		deleteAction->setEnabled(false);
		saveAction->setEnabled(false);
	}
}

bool RdbViewFrame::openRdbDatabase()
{
	if (!m_dbPtr->open("serverdb1"))
	{
		emit databaseOpenState(false);
		QMessageBox::critical(this, QStringLiteral("实时库操作"), QStringLiteral("数据库打开失败"));
		return false;
	}

	emit databaseOpenState(true);
	return true;
}

bool RdbViewFrame::closeRdbDatabase()
{
	m_dbPtr->close();
	emit databaseOpenState(false);
	return true;
}

bool RdbViewFrame::addData()
{
	if (m_tableOperPtr)
	{
		QString tableName;
		if (!getCurrentTableName(tableName))
		{
			return false;
		}
		if (m_tableOperPtr->insertData(tableName))
		{
			refreshData();
		}
		else
		{
			QMessageBox::critical(this, QStringLiteral("实时库操作"), QStringLiteral("插入数据失败"));
		}
	}
	else
	{
		QMessageBox::critical(this, QStringLiteral("实时库操作"), QStringLiteral("实时库操作指针为空"));
		return false;
	}
	return true;
}

bool RdbViewFrame::deleteData()
{
	if (m_tableOperPtr)
	{
		// 获取数据表名
		QString tableName;
		if (!getCurrentTableName(tableName))
		{
			return false;
		}

		// 获取当前数据的mRID
		QString mRID;
		if (!getCurrentMRID(mRID))
		{
			return false;
		}
		
		if (m_tableOperPtr->deleteData(tableName, mRID))
		{
			refreshData();
		}
		else
		{
			QMessageBox::critical(this, QStringLiteral("实时库操作"), QStringLiteral("删除数据失败"));
		}
	}
	else
	{
		QMessageBox::critical(this, QStringLiteral("实时库操作"), QStringLiteral("实时库操作指针为空"));
		return false;
	}
	return true;
}

bool RdbViewFrame::saveData()
{
	if (m_tableOperPtr)
	{
		// 获取数据表名
		QString tableName;
		if (!getCurrentTableName(tableName))
		{
			return false;
		}

		// 获取当前数据的mRID
		QString mRID;
		if (!getCurrentMRID(mRID))
		{
			return false;
		}

		// 获取当前行的所有记录
		QTableWidgetItem* item = m_rdbTableWidget->currentItem();
		if (item)
		{
			QMap<QString, QString> values;
			int columnCount = m_rdbTableWidget->columnCount();
			for (int i = 0; i < columnCount; ++i)
			{
				QString fieldName = m_rdbTableWidget->horizontalHeaderItem(i)->text();
				QString value = m_rdbTableWidget->item(item->row(), i)->text();
				values.insert(fieldName, value);
			}
			if (!m_tableOperPtr->saveData(tableName, mRID, values))
			{
				QMessageBox::critical(this, QStringLiteral("实时库操作"), QStringLiteral("删除数据失败"));
				refreshData();
			}
		}
	}
	else
	{
		QMessageBox::critical(this, QStringLiteral("实时库操作"), QStringLiteral("实时库操作指针为空"));
		return false;
	}
	return true;
}

bool RdbViewFrame::refreshData()
{
	QTreeWidgetItem* item = m_rdbTreeWidget->currentItem();
	refreshDatas(item);
	return true;
}

bool RdbViewFrame::deleteAllData()
{
	if (m_tableOperPtr)
	{
		// 获取数据表名
		QString tableName;
		if (!getCurrentTableName(tableName))
		{
			return false;
		}

		if (m_tableOperPtr->deleteAllData(tableName))
		{
			refreshData();
		}
		else
		{
			QMessageBox::critical(this, QStringLiteral("实时库操作"), QStringLiteral("删除数据失败"));
		}
	}
	else
	{
		QMessageBox::critical(this, QStringLiteral("实时库操作"), QStringLiteral("实时库操作指针为空"));
		return false;
	}
	return true;
}

void RdbViewFrame::hideTableColumn()
{
	if (!m_rdbTableWidget)
	{
		return;
	}

	HideColumnDialog hideDlg(m_rdbTableWidget, this);
	if(hideDlg.exec() == QDialog::Accepted)
	{
		QList<int> hideColumns = hideDlg.getHideColumns();
		foreach(int i, hideColumns)
		{
			m_rdbTableWidget->hideColumn(i);
		}
	}
}

void RdbViewFrame::about()
{
	QMessageBox::information(this, QStringLiteral("实时库操作"), QStringLiteral("实时库浏览操作应用程序"));
}


