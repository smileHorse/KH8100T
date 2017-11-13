#ifndef RDBFRAME_H
#define RDBFRAME_H

#include "fastdb.h"

#include <QtWidgets/QMainWindow>

class QTreeWidget;
class QTableWidget;
class QAction;
class QMenu;
class QToolBar;
class QTreeWidgetItem;
class QTableWidgetItem;
class TableOperation;

#pragma execution_character_set("utf-8")

class RdbViewFrame : public QMainWindow
{
	Q_OBJECT

public:
	RdbViewFrame(QWidget *parent = 0);
	~RdbViewFrame();

protected:

private:
	void createWidgets();
	void createActions();
	void createMenus();
	void createToolbars();
	void createStatusBar();
	void createConnects();

	void refreshDatas(QTreeWidgetItem* item);

	bool getCurrentTableName(QString& tableName);
	bool getCurrentMRID(QString& mRID);
	bool getCurrentTableItemValue(QString& value);
	bool getCurrentTableHeaderLabel(QString& value);

signals:
	void databaseOpenState(bool opened);

public slots:

private slots:
	bool openRdbDatabase();
	bool closeRdbDatabase();
	bool addData();
	bool deleteData();
	bool saveData();
	bool refreshData();
	bool deleteAllData();
	void hideTableColumn();
	void about();
	void updateDatabaseActions(bool opened);
	void updateTreeWidget();
	void updateTableWidget(QTreeWidgetItem* current, QTreeWidgetItem* previous);
	void treeItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
	void tableItemChanged(QTableWidgetItem* current, QTableWidgetItem* previous);

private:
	dbDatabase*	m_dbPtr;
	TableOperation*	m_tableOperPtr;

	QTreeWidget*	m_rdbTreeWidget;
	QTableWidget*	m_rdbTableWidget;

	QAction*	openAction;
	QAction*	closeAction;
	QAction*	exitAction;
	QAction*	addAction;
	QAction*	deleteAction;
	QAction*	saveAction;
	QAction*	refreshAction;
	QAction*	deleteAllAction;
	QAction*	hideTableColumnAction;
	QAction*	aboutAction;

	QMenu*	fileMenu;
	QMenu*	editMenu;
	QMenu*	helpMenu;
	QToolBar*	fileToolBar;
	QToolBar*	editToolBar;
};

#endif // RDBFRAME_H
