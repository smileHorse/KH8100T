
#include <stdio.h>

#include "transferFileByCThread.h"
#include "transferFileCommon.h"
#include "transferFileNew.h"
#include "transferfileserver.h"

TransferFileByCThread::TransferFileByCThread(TransferFileServer* frame,  const QString& filePath, QObject* parent /*= 0*/ )
	: QThread(parent), _frame(frame), _filePath(filePath)
{

}

void TransferFileByCThread::run()
{
	// 获取传输文件的Ice代理
	TransferFileSpace::TransferFilePrx transferFilePrx = 0;
	if (_frame->getTransferMode() == Transfer_Ice)
	{
		transferFilePrx = _frame->getTransferFilePrx();
	}
	else
	{
		transferFilePrx = _frame->getTransferFilePublisher();
	}
	if (transferFilePrx == 0)
	{
		emit emitExecuteInfo(QStringLiteral("获取传输文件接口失败: 传输接口为空"));
		return;
	}

	// 打开文件
	FILE* inFile = NULL;
	if ((inFile = fopen(_filePath.toLatin1(), "r+b")) == NULL)
	{
		emit emitExecuteInfo(QStringLiteral("打开文件 %1 失败").arg(_filePath));
		return;
	}

	try
	{
		// 读取文件内容并发送
		char buffer[TransferBufferSize] = { 0 };
		int count = 0;
		QString fileName = _filePath.right(_filePath.length() - _filePath.lastIndexOf('/') - 1);
		while((count = fread(buffer, sizeof(char), TransferBufferSize, inFile)) > 0)
		{
			TransferFileSpace::Bytes fileDatas;
			fileDatas.resize(count);
			for (int i = 0; i < count; ++i)
			{
				fileDatas[i] = buffer[i];
			}
			transferFilePrx->sendFile(fileName.toStdString(), fileDatas);
		}

		if (count < 0)
		{
			emit emitExecuteInfo(QStringLiteral("文件 %1 发送失败: 读取文件时发送错误").arg(_filePath));
		}
		else
		{
			emit emitExecuteInfo(QStringLiteral("文件 %1 发送成功").arg(_filePath));
		}
	}
	catch(const Ice::Exception& ex)
	{
		emit emitExecuteInfo(QStringLiteral("文件 %1 发送失败: %2").arg(_filePath).arg(ex.what()));
	}	

	// 关闭文件
	fclose(inFile);
}
