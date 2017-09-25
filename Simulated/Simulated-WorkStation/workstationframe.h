#ifndef WORKSTATIONFRAME_H
#define WORKSTATIONFRAME_H

#include "OperationInfo.h"

#include <QtWidgets/QMainWindow>

class QTableWidget;
class QTextEdit;
class QAction;
class QMenu;
class QToolBar;
class WorkStationServer;
class WorkStationServerThread;

#pragma execution_character_set("utf-8")

class WorkStationFrame : public QMainWindow
{
	Q_OBJECT

public:
	WorkStationFrame(int argc, char* argv[], QWidget *parent = 0);
	~WorkStationFrame();

private:
	void createWidgets();

	void createActions();
	void createMenus();
	void createToolBars();
	void createStatusBar();
	void createConnects();

signals:
	void serverStarted(bool serverStarted);

public slots:
	void updateTableWidget(const OperationInfo& info);

private slots:
	void updateActions(bool serverStarted);
	void startServer();
	void stopServer();
	void requestStormRdbData();
	void requestStormTopoData();
	void requestRdbData();
	void requestTopoData();
	void about();


private:
	int	m_argc;
	char**	m_argv;

	WorkStationServer*	m_workStationServerPtr;
	WorkStationServerThread*	m_workStationServerThreadPtr;

	QTableWidget*	tableWidget;
	QTextEdit*		textEdit;

	QAction*	startServerAction;
	QAction*	stopServerAction;
	QAction*	exitAction;
	QAction*	requestStormRdbDataAction;
	QAction*	requestStormTopoDataAction;
	QAction*	requestRdbDataAction;
	QAction*	requestTopoDataAction;
	QAction*	helpAction;

	QMenu*		fileMenu;
	QMenu*		operMenu;
	QMenu*		helpMenu;
	QToolBar*	fileToolBar;
	QToolBar*	operToolBar;

};


#endif // WORKSTATIONFRAME_H
