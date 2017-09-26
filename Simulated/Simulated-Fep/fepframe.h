#ifndef FEPFRAME_H
#define FEPFRAME_H

#include <QtWidgets/QMainWindow>

class QTableWidget;
class QAction;
class QMenu;
class QToolBar;

class FepServer;
class FepServerThread;

class OperationInfo;

#pragma execution_character_set("utf-8")

class FepFrame : public QMainWindow
{
	Q_OBJECT

public:
	FepFrame(int argc, char* argv[], QWidget *parent = 0);
	~FepFrame();

public slots:
	void startServer();

private:
	void createWidgets();

	void createActions();
	void createMenus();
	void createToolBars();
	void createStatusBar();

private slots:
	void about();
	void updateTableWidget(const OperationInfo& text);

private:
	FepServer*	m_fepServerPtr;
	FepServerThread*	m_fepServerThreadPtr;


	QTableWidget*	tableWidget;

	QAction*	exitAction;
	QAction*	processDataAction;
	QAction*	processFaultAction;
	QAction*	processEventAction;
	QAction*	processWaveAction;
	QAction*	helpAction;

	QMenu*		fileMenu;
	QMenu*		operMenu;
	QMenu*		helpMenu;
	QToolBar*	fileToolBar;
	QToolBar*	operToolBar;
};

#endif // FEPFRAME_H
