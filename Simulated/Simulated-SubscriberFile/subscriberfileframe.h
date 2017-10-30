#ifndef SUBSCRIBERFILEFRAME_H
#define SUBSCRIBERFILEFRAME_H

#include "transferfileserver.h"

#include <QtWidgets/QMainWindow>

class QAction;
class QMenu;
class QToolBar;
class QTextEdit;

class SubscriberFileFrame : public QMainWindow
{
	Q_OBJECT

public:
	SubscriberFileFrame(QWidget *parent = 0);
	~SubscriberFileFrame();

	void setTransFileServer(const QSharedPointer<TransferFileServer>& server);

private:
	void createWidgets();
	void createActions();
	void createConnectes();
	void createMenus();
	void createToolBars();
	void createStatusBar();
	void updateActions();

signals:
	void subscriberFileSignal(bool isStart);

private slots:
	void subscriberFile();
	void clearTextEdit();
	void updateTextEdit(QString text);

private:
	QTextEdit*	textEdit;
	QAction*	closeAction;
	QAction*	subscriberAction;
	QAction*	clearAction;

	QMenu*		fileMenu;
	QMenu*		operMenu;

	QToolBar*	fileToolBar;
	QToolBar*	operToolBar;

	bool		m_subscribering;

	QSharedPointer<TransferFileServer>	m_transFileServer;
};

#endif // SUBSCRIBERFILEFRAME_H
