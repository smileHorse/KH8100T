#ifndef WORKSTATIONFRAME_H
#define WORKSTATIONFRAME_H


#include <QtWidgets/QMainWindow>

class QTableWidget;
class QTextEdit;
class QAction;
class QMenu;
class QToolBar;
class WorkStationServer;
class WorkStationServerThread;
class OperationInfo;

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
	QAction* createActionImpl(const QIcon &icon, const QString &text, const QString& statusTip, 
		const QObject* receiver, const char* member, bool enabled = true);
	void createMenus();
	void createToolBars();
	void createStatusBar();
	void createConnects();

	void updateActionText(QAction* action, bool& isStop);

signals:
	void serverStarted(bool serverStarted);
	void requestCompleteDataSingal();
	void requestStormTopoDataSingal();
	void selectCompleteDataSingal();
	void requestWarningMsgSingal();
	void subscriberRdbRequestSignal(bool isStop);
	void subscriberRdbRespondSignal(bool isStop);
	void subscriberAlarmDataSignal(bool isStop);
	void subscriberFepDataSignal(bool isStop);
	void subscriberYkFepSignal(bool isStop);
	void subscriberYkAppSignal(bool isStop);
	void subscriberWarningMsgSignal(bool isStop);
	void ykSelectSignal(bool isStop);
	void transferCurveFileSignal(QString date, QString fileName);
	void transferWarningFileSignal(QString fileName);
	void getSubscribersSignal(QString topicName);
	void getPublishersSignal(QString topicName);

public slots:
	void updateTableWidget(const OperationInfo& info);
	void updateTextEdit(const QString& text);

private slots:
	void updateActions(bool serverStarted);
	void startServer();
	void stopServer();
	void requestStormRdbData();
	void requestStormTopoData();
	void requestRdbData();
	void requestTopoData();
	void requestWarningMsg();
	void subscriberRdbRequest();
	void subscriberRdbRespond();
	void subscriberAlarmData();
	void subscriberFepData();
	void subscriberYkFep();
	void subscriberYkApp();
	void subscribeWarningMsg();
	void ykSelect();
	void transferCurveFile();
	void transferWarningFile();
	void getSubscribers();
	void getPublishers();
	void clearTextEdit();
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
	QAction*	requestWarningMsgAction;
	QAction*	subscriberRdbRequestAction;
	QAction*	subscriberRdbRespondAction;
	QAction*	subscriberAlarmDataAction;
	QAction*	subscriberFepDataAction;
	QAction*	subscriberYkFepAction;
	QAction*	subscriberYkAppAction;
	QAction*	subscriberWarningMsgAction;
	QAction*	ykSelectAction;
	QAction*	transferCurveFileAction;
	QAction*	transferWarningFileAction;
	QAction*	getSubscribersAction;
	QAction*	getPublishersAction;
	QAction*	clearAction;
	QAction*	helpAction;

	QMenu*		fileMenu;
	QMenu*		operMenu;
	QMenu*		helpMenu;
	QToolBar*	fileToolBar;
	QToolBar*	operToolBar;
};


#endif // WORKSTATIONFRAME_H
