#ifndef AMSFRAME_H
#define AMSFRAME_H

#include <QtWidgets/QMainWindow>

class QTableWidget;
class QAction;
class QMenu;
class QToolBar;
class OperationInfo;

class AmsFrame : public QMainWindow
{
	Q_OBJECT

public:
	AmsFrame(QWidget *parent = 0);
	~AmsFrame();

public:
	void setParam(int argc, char* argv[]);
	
private:
	void createWidgets();
	void createActions();
	void createMenus();
	void createToolbar();
	void createStatusBar();

	void hasSetRole(bool isMaster);

signals:
	void setRole(QString role);
	void changeRole(QString role);
	void configHostPort(QString host, int port);

public slots:
	// 开始执行服务
	void startServer();

private slots:
	void about();
	void updateTableWidget(const OperationInfo& info);
	void setMasterRole();
	void setSlaveRole();
	void changeRole();
	void configHostPort();

private:
	int		m_argc;
	char**	m_argv;

	QTableWidget*	tableWidget;

	QAction*	exitAction;
	QAction*	setMasterAction;
	QAction*	setSlaveAction;
	QAction*	changeRoleAction;
	QAction*	configAction;
	QAction*	helpAction;

	QMenu*	fileMenu;
	QMenu*	operMenu;
	QMenu*	helpMenu;

	QToolBar*	fileToolbar;
	QToolBar*	operToolbar;
};

#endif // AMSFRAME_H
