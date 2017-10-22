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

public slots:
	// 开始执行服务
	void startServer();

private:
	void createWidgets();

	void createActions();

	void createMenus();
	void createToolbar();
	void createStatusBar();

private slots:
	void about();
	void updateTableWidget(const OperationInfo& info);

private:
	int		m_argc;
	char**	m_argv;

	QTableWidget*	tableWidget;

	QAction*	exitAction;
	QAction*	helpAction;

	QMenu*	fileMenu;
	QMenu*	helpMenu;

	QToolBar*	fileToolbar;
};

#endif // AMSFRAME_H
