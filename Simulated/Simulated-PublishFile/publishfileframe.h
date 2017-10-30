#ifndef PUBLISHFILEFRAME_H
#define PUBLISHFILEFRAME_H

#include "transferfileserver.h"

#include <string>
#include <QtWidgets/QMainWindow>

using namespace std;

class QAction;
class QMenu;
class QToolBar;
class QTextEdit;

class PublishFileFrame : public QMainWindow
{
	Q_OBJECT

public:
	PublishFileFrame(QWidget *parent = 0);
	~PublishFileFrame();

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
	void publishFileSignal(bool isStart);

private slots:
	void publishFile();
	void readFile();
	void writeFile();
	void clearTextEdit();
	void updateTextEdit(QString text);

private:
	QTextEdit*	textEdit;
	QAction*	closeAction;
	QAction*	publishAction;
	QAction*	readFileAction;
	QAction*	writeFileAction;
	QAction*	clearAction;

	QMenu*		fileMenu;
	QMenu*		operMenu;

	QToolBar*	fileToolBar;
	QToolBar*	operToolBar;

	bool		m_publishing;

	string		m_fileContent;

	QSharedPointer<TransferFileServer>	m_transFileServer;
};

#endif // PUBLISHFILEFRAME_H
