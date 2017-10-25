#ifndef RESTOREDB_H
#define RESTOREDB_H

#include <QtWidgets/QMainWindow>

class QAction;
class QMenu;
class QToolBar;
class QTextEdit;

class FastdbManager;

class RestoreDb : public QMainWindow
{
	Q_OBJECT

public:
	RestoreDb(QWidget *parent = 0);
	~RestoreDb();

private:
	void createWidgets();
	void createActions();
	void createConnects();
	void createMenus();
	void createToolBars();
	void createStatusBar();

private slots:
	void openDb();
	void restoreDb();
	void clearTextEdit();
	void outputOperationInfo(QString text);

private:
	FastdbManager*	m_fastdbManager;

	QTextEdit*	textEdit;
	QAction*	openAction;
	QAction*	closeAction;
	QAction*	restoreAction;
	QAction*	clearAction;

	QMenu*		fileMenu;
	QMenu*		operMenu;
	QToolBar*	fileToolbar;
	QToolBar*	operToolbar;

};

#endif // RESTOREDB_H
