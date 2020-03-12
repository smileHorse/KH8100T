#ifndef FEPFRAME_H
#define FEPFRAME_H

#include <QtWidgets/QMainWindow>

class QTableWidget;
class QTextEdit;
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
	void updateTextEdit(const QString& text);
	void clearTextEdit();

	void processUnitStateData();
	void processYxTypeEvent();
	void processSoeTypeEvent();
	void processProTypeEvent();

	void configFaultEvent();

private:
	FepServer*	m_fepServerPtr;
	FepServerThread*	m_fepServerThreadPtr;


	QTableWidget*	tableWidget;
	QTextEdit*		textEdit;

	QAction*	exitAction;
	QAction*	processYxDataAction;
	QAction*	processYcDataAction;
	QAction*	processDdDataAction;
	QAction*	processUnitStateDataAction;
	QAction*	processDLFaultAction;
	QAction*	processJDFaultAction;
	QAction*	processYxTypeEventAction;
	QAction*	processSoeTypeEventAction;
	QAction*	processUnitTypeEventAction;
	QAction*	processUnitChannelEventAction;
	QAction*	processProTypeEventAction;
	QAction*	processWaveAction;
	QAction*	clearAction;
	QAction*	helpAction;

	QMenu*		fileMenu;
	QMenu*		operMenu;
	QMenu*		helpMenu;
	QToolBar*	fileToolBar;
	QToolBar*	operToolBar;
};

#endif // FEPFRAME_H
