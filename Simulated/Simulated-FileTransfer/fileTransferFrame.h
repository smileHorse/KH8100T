#ifndef FILETRANSFERFRAME_H
#define FILETRANSFERFRAME_H

#include "iceServer.h"
#include "TransferFileItem.h"

#include <QtWidgets/QMainWindow>

class QAction;
class QTableWidget;
class QProgressBar;

class FileTransferFrame : public QMainWindow
{
	Q_OBJECT

public:
	FileTransferFrame(int argc, char* argv[], QWidget* parent = 0);

private:
	void createWidgets();
	void createActions();
	void createConnections();
	void createMenus();
	void createToolBars();
	void createStatusBar();
	void updateUi();

private slots:
	void configLocal();
	void startLocalServer();
	void stopLocalServer();
	void configRemote();
	void uploadFile();
	void downloadFile();
	void outputMessage(const QString& msg);
	void updateStatusBar(const QString& msg);
	void updateTableWidget(const TransferFileItem& item);
	void clearTabelWidget();
	void serverStarted(bool status, const QString& msg);
	void updateProgress(int value);

private:
	QAction* quitAction;

	QAction* configLocalAction;		// 配置本地的ip和port
	QAction* startClientAction;
	QAction* stopClientAction;

	QAction* configRemoteAction;		// 配置对端的ip和port
	QAction* uploadFileAction;
	QAction* downloadFileAction;

	QAction* clearAction;

	QTableWidget*	tableWidget;
	QProgressBar*	progressBar;
	
	bool	m_serverStarted;

	IceServer m_iceServer;
};
#endif