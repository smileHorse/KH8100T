
#include "FileHandler.h"

#include <fstream>
#include <iterator>

using namespace std;

CFileHandler::CFileHandler()
{

}

// 打开文件
bool CFileHandler::openFile( const string& fileName )
{
	return true;
}

// 关闭文件
void CFileHandler::closeFile()
{

}

// 读取文件
bool CFileHandler::readFile( const string& fileName, string& content )
{	
	QString filePath = getReadFilePath(fileName);
	QFile file(filePath);
	if (file.open(QIODevice::ReadOnly)) {
		QString data(file.readAll());		
		content = data.toStdString();

		file.close();
		return true;
	}
	else
	{
		return false;
	}

	


	return true;
}

bool CFileHandler::readFileWithFStream( const string& fileName, string& content )
{
	QString filePath = getReadFilePath(fileName);
	//ifstream inputFile(filePath.toStdString());
	//inputFile.unsetf(ios::skipws);
	//string fileData((istream_iterator<char>(inputFile)), istream_iterator<char>());
	//content = fileData;

	std::ifstream fin(filePath.toStdString(), std::ios::binary);
	std::ofstream fout(getWriteFilePath(fileName).toStdString(), std::ios::binary | std::ios::trunc);

	int nNum;
	char szBuf[256] = {0};

	bool result = true;
	while(!fin.eof())  
	{  
		char szBuf[256] = {0};  

		fin.read(szBuf, sizeof(char) * 256);  

		if (fout.bad())
		{
			result = false;
			break;
		}
		
		fout.write(szBuf, sizeof(char) * 256);

		content.append(szBuf);
	}  

	fin.close();

	return true;
}

bool CFileHandler::readFileWithQFile( const string& fileName, string& content )
{
	QString filePath = getReadFilePath(fileName);
	QFile file(filePath);
	if (file.open(QIODevice::ReadOnly)) {
		QString data(file.readAll());		
		content = data.toStdString();

		file.close();
		return true;
	}
	else
	{
		return false;
	}
}

bool CFileHandler::readFileWithQTextStream( const string& fileName, string& content )
{
	QString filePath = getReadFilePath(fileName);
	QFile file(filePath);
	if ( file.open(QFile::ReadOnly ) ) {
		QTextStream inStream(&file);

		QString line;
		do {
			line = inStream.read(75);
			content.append(line.toStdString());
		} while ( !line.isNull() ) ;
		file.close();

		return true;
	}
	else
	{
		return false;
	}
}

bool CFileHandler::readFileWithQDataStream( const string& fileName, string& content )
{
	QString filePath = getReadFilePath(fileName);
	QFile file(filePath);
	if ( file.open(QFile::ReadOnly ) ) {
		QDataStream inStream(&file);

		char* buffer = new char[1000];
		unsigned int size;
		inStream.device()->reset();
		while((size = inStream.readRawData(buffer, 1000)) != 0)
		{
			content.append(buffer);
		}
		file.close();

		return true;
	}
	else
	{
		return false;
	}
}

// 写入文件
bool CFileHandler::writeFile( const string& fileName, const string& content )
{
	QString filePath = getWriteFilePath(fileName);
	QFile file(filePath);
	if ( file.open(QFile::WriteOnly | QFile::Truncate) ) {
		QTextStream outStream(&file);
		outStream << QString().fromStdString(content);
	}
	file.close();

	return true;
}

QString CFileHandler::getReadFilePath( const string& fileName )
{
	return "source/" + QString().fromStdString(fileName);
}

QString CFileHandler::getWriteFilePath( const string& fileName )
{
	return "target/" + QString().fromStdString(fileName);
}
