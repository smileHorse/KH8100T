
#include "iceServer.h"
#include "TransferFileI.h"

#include <string>

using namespace std;
using namespace TransferFileModule;

IceServer::IceServer(QObject* parent)
	: QObject(parent)
{
	m_transferFileIdentity = "transferFile";
}

int IceServer::run( int argc, char* argv[] )
{
	try 
	{
		m_communicatorPtr = communicator();

		string endPoints = QString("tcp -h %1 -p %2 -t 3000").arg(m_localIp).arg(m_localPort).toStdString();
		m_objectAdapterPtr = m_communicatorPtr->createObjectAdapterWithEndpoints("iceClient", endPoints);
		
		m_transferFileManagerPrx = m_objectAdapterPtr->add(new TransferFileManagerI(), 
			m_communicatorPtr->stringToIdentity(m_transferFileIdentity));

		m_objectAdapterPtr->activate();

		m_communicatorPtr->waitForShutdown();
	}
	catch(const Ice::Exception& ex)
	{
		emit serverStarted(false, QStringLiteral("启动Ice服务失败: %1").arg(QString().fromStdString(ex.what())));
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

void IceServer::setParam( int argc, char* argv[] )
{
	m_argc = argc;
	m_argv = argv;
}

void IceServer::start()
{
	main(m_argc, m_argv);
}

void IceServer::shutdown()
{
	m_communicatorPtr->shutdown();
}

void IceServer::setLocalHost( const QString& host, int port )
{
	m_localIp = host;
	m_localPort = port;
}

void IceServer::setRemoteHostPort( const QString& host, int port )
{
	m_remoteIp = host;
	m_remotePort = port;
}

void IceServer::uploadFile( const QString& fileName )
{
	QString proxy = getTransferFileProxy();
	try 
	{
		TransferFileManagerPrx transferFilePrx = TransferFileManagerPrx::checkedCast(
			m_communicatorPtr->stringToProxy(proxy.toStdString()));
		
		FILE* inf;
		if ((inf = fopen(fileName.toStdString().c_str(), "r+b")) == NULL)
		{
			emit emitMessage(QStringLiteral("上传文件失败：无法打开文件 %1").arg(fileName));
			return;
		}

		QTime startTime;
		startTime.start();
		long sizeCount = 0;

		fseek(inf, 0, SEEK_END);
		long maxValue = ftell(inf);
		fseek(inf, 0, SEEK_SET);

		QString tempFileName = fileName;
		tempFileName = tempFileName.replace("\\", "/");
		tempFileName = tempFileName.right(tempFileName.length() - tempFileName.lastIndexOf('/') - 1);
		string filePath = QString("%1%2").arg(UPLOAD_PATH).arg(tempFileName).toStdString();
		int count = 0;
		while(!feof(inf))
		{
			char buffer[FILE_BUFFER_SIZE] = {0};
			int nChars = fread(buffer, sizeof(char), FILE_BUFFER_SIZE, inf);
			
			if (nChars)
			{
				Bytes content(buffer, buffer + nChars);
				transferFilePrx->uploadFile(filePath, content);
				sizeCount += nChars;

				emit emitProgress( (ftell(inf) * 100 / maxValue) );
			}
			++count;
		}		

	//	emit emitMessage(QStringLiteral("读取文件次数 %1").arg(count));
		

		fclose(inf);

		TransferFileItem item;
		item.operMode = UploadFile;
		item.fileName = fileName;
		item.fileSize = item.calcFileSize(sizeCount);
		item.transMode = QStringLiteral("Ice");
		item.transTime = item.convertTime(startTime.elapsed());
		emit emitTransferFileItem(item);
	}
	catch(const Ice::Exception& ex)
	{
		emit emitMessage(QStringLiteral("上传文件失败，获取远方服务异常: %1").arg(QString().fromStdString(ex.what())));
	}
}

void IceServer::downloadFile( const QString& fileName )
{
	QString proxy = getTransferFileProxy();
	try 
	{
		TransferFileManagerPrx transferFilePrx = TransferFileManagerPrx::checkedCast(
			m_communicatorPtr->stringToProxy(proxy.toStdString()));

		FILE* outf;
		string filePath = QString("%1%2").arg(DOWNLOAD_PATH).arg(fileName).toStdString();
		if ((outf = fopen(filePath.c_str(), "w+b")) == NULL)
		{
			emit emitMessage(QStringLiteral("下载文件失败: 无法打开文件 %1").arg(fileName));
			return;
		}
		
		QTime startTime;
		startTime.start();
		long sizeCount = 0;

		FileContent fileContent;
		fileContent.pos = 0;
		do 
		{
			fileContent = transferFilePrx->downloadFile(filePath, fileContent.pos);
			int size = fileContent.content.size();
			if (size > 0)
			{
				char* buffer = new char[size];
				memcpy(buffer, &(fileContent.content), size);
				fwrite(buffer, sizeof(char), size, outf);
				delete [] buffer;

				sizeCount += size;
			}
		} while (!fileContent.content.empty());	

		fclose(outf);

		TransferFileItem item;
		item.operMode = DownloadFile;
		item.fileName = QString().fromStdString(filePath);
		item.fileSize = item.calcFileSize(sizeCount);
		item.transMode = QStringLiteral("Ice");
		item.transTime = item.convertTime(startTime.elapsed());
		emit emitTransferFileItem(item);
	}
	catch(const Ice::Exception& ex)
	{
		emit emitMessage(QStringLiteral("下载文件失败: 获取远方服务异常: %1").arg(QString().fromStdString(ex.what())));
	}
}

QString IceServer::getTransferFileProxy()
{
	QString proxy = QString("%1: tcp -h %2 -p %3 -t 3000").arg(QString().fromStdString(m_transferFileIdentity)).
		arg(m_remoteIp).arg(m_remotePort);

	return proxy;
}
