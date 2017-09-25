#ifndef FEPFRAME_H
#define FEPFRAME_H

#include <QtWidgets/QMainWindow>

class QListWidget;
class QAction;
class QMenu;
class QToolBar;

class FepServer;
class FepServerThread;

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
	void updateListWidget(const QString& text);

private:
	FepServer*	m_fepServerPtr;
	FepServerThread*	m_fepServerThreadPtr;


	QListWidget*	listWidget;

	QAction*	exitAction;
	QAction*	processDataAction;
	QAction*	helpAction;

	QMenu*		fileMenu;
	QMenu*		operMenu;
	QMenu*		helpMenu;
	QToolBar*	fileToolBar;
	QToolBar*	operToolBar;
};

#endif // FEPFRAME_H
