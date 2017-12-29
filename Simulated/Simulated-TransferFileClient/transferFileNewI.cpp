
#include "transferfileclient.h"
#include "transferFileNewI.h"

TransferFileI::TransferFileI( TransferFileClient* frame )
	: _frame(frame)
{
	_textes.clear();
}

void TransferFileI::sendFile( const ::std::string& filePath, const ::TransferFileSpace::Bytes& fileDatas, 
		const ::Ice::Current& /* = ::Ice::Current() */ )
{
	if (_frame)
	{
		if (_frame->getFileOperInterface() == FileInter_C)
		{
			saveFile_C(filePath, fileDatas);
		}
		else
		{
			saveFile_CPlusPlus(filePath, fileDatas);
		}
	}
}

void TransferFileI::saveFile_C( const ::std::string& filePath, const ::TransferFileSpace::Bytes& fileDatas )
{
	FILE* outFile = 0;
	if ((outFile == fopen(filePath.c_str(), "ab+")) == NULL)
	{
		_textes.append(QStringLiteral("打开文件 %1 失败").arg(QString().fromStdString(filePath)));
		return;
	}

	size_t bufferSize = fileDatas.size();
	char* buffer = new char[bufferSize];
	for (size_t i = 0; i < bufferSize; ++i)
	{
		buffer[i] = (char)fileDatas[i];
	}
	if (fwrite(buffer, sizeof(char), bufferSize, outFile) == bufferSize)
	{
		_textes.append(QStringLiteral("保存文件 %1 成功").arg(QString().fromStdString(filePath)));
	}
	else
	{
		_textes.append(QStringLiteral("保存文件 %1 失败").arg(QString().fromStdString(filePath)));
	}
	delete [] buffer;
}

void TransferFileI::saveFile_CPlusPlus( const ::std::string& filePath, const ::TransferFileSpace::Bytes& fileDatas )
{

}

void TransferFileI::updateFrameTextEdit()
{
	if (_frame)
	{
		foreach(const QString& text, _textes)
		{
			_frame->updateTextEdit(text);
		}
	}
}
