/********************************************************************
	created:	2017/12/27
	created:	27:12:2017   15:41
	filename: 	F:\code\GitProject\KH8100T\Simulated\Simulated-TransferFileServer\transferfileserver.h
	file path:	F:\code\GitProject\KH8100T\Simulated\Simulated-TransferFileServer
	file base:	transferfileserver
	file ext:	h
	author:		Masf
	
	purpose:	文件传输的服务端 -- 用于主动发送文件和被动请求文件
*********************************************************************/
#ifndef TRANSFERFILESERVER_H
#define TRANSFERFILESERVER_H

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <IceStorm/IceStorm.h>
#include <QtWidgets/QMainWindow>

#include "configIceDialog.h"
#include "transferFileCommon.h"
#include "transferFileNew.h"

class QAction;
class QTextEdit;

class TransferFileServer : public QMainWindow
{
	Q_OBJECT

public:
	TransferFileServer(QWidget *parent = 0);
	~TransferFileServer();

private:
	void createWidgets();
	void createActions();
	QAction* createActionImpl(const QIcon& icon, const QString& text, const QString& statusTip, 
		const char* slot);
	void createMenus();
	void createToolBars();
	void createStatusBar();
	void updateUi();
	void transferFile(const QString& filePath);
	void transferFile_C(const QString& filePath);
	void transferFile_CPlusPlus(const QString& filePath);

private slots:
	void useIce();
	void useIceStorm();
	void useText();
	void useBinary();
	void useC();
	void useCPlusPlus();
	void selectFile();
	void clearTextEdit();
	void configIceServer();
	void startIceServer();
	void stopIceServer();
	void updateTextEdit(const QString& text);

public:
	TransferMode getTransferMode() const;
	FileOperMode getFileOperMode() const;
	FileOperInterface getFileOperInterface() const;
	TransferFileSpace::TransferFilePrx getTransferFilePrx();
	TransferFileSpace::TransferFilePrx getTransferFilePublisher();

private:
	QAction*	iceAction;			// 通过Ice进行传输文件
	QAction*	iceStormAction;		// 通过IceStorm进行传输文件

	QAction*	textAction;			// 传输文本文件
	QAction*	binaryAction;		// 传输二进制文件

	QAction*	useCAction;			// 使用C标准接口操作文件
	QAction*	useCPlusPlusAction;	// 使用C++标准接口操作文件

	QAction*	selectFileAction;		// 选择发送的文件
	QAction*	clearTextEditAction;	// 清空文本框
	QAction*	closeAction;			// 关闭程序

	QAction*	configIceAction;	// 配置Ice服务
	QAction*	startIceAction;		// 启动Ice服务
	QAction*	stopIceAction;		// 停止Ice服务

	QTextEdit*	textEdit;		// 操作信息文本框

	TransferMode		transferMode;		// 文件传输方式
	FileOperMode		fileOperMode;		// 文件读写格式
	FileOperInterface	fileOperInterface;	// 文件操作接口

	Ice::CommunicatorPtr	m_communicatorPtr;	// ice连接器
	Ice::ObjectAdapterPtr	m_objectAdapterPtr;	// ice配接器

	QString	adapterIdentify;	// 适配器标识
	ConfigIceInfo	configIceInfo;	// Ice服务配置信息

	TransferFileSpace::TransferFilePrx	m_transferFilePrx;			// 传输文件的Ice代理
	TransferFileSpace::TransferFilePrx	m_transferFilePublisher;	// 传输文件的IceStorm发布代理
};

#endif // TRANSFERFILESERVER_H
