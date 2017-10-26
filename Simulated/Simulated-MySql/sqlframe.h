#ifndef SQLFRAME_H
#define SQLFRAME_H

#include "ExecThread.h"

#include <QtCore/QtCore>
#include <QtWidgets/QMainWindow>

class QAction;
class QMenu;
class QToolBar;
class QTextEdit;

class SqlFrame : public QMainWindow
{
	Q_OBJECT

public:
	SqlFrame(QWidget *parent = 0);
	~SqlFrame();

private:
	void createWidgets();
	void createActions();
	void createConnectes();
	void createMenus();
	void createToolBars();
	void createStatusBar();

	void updateExecAction(bool isStart);

private slots:
	void startExecSql();
	void stopExecSql();
	void clearTextEdit();
	void updateTextEdit(QString text);

private:
	QTextEdit*	textEdit;

	QAction*	startAction;
	QAction*	stopAction;
	QAction*	closeAction;
	QAction*	clearAction;

	QMenu*		fileMenu;
	QMenu*		operMenu;

	QToolBar*	fileToolbar;
	QToolBar*	operToolbar;

	QVector< QSharedPointer<CExecThread> > m_execThreads;
};

#endif // SQLFRAME_H
