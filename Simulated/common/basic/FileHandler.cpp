
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
	QString fileTitle = "source/" + QString().fromStdString(fileName);
	QFile file(fileTitle);
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

	

	//ifstream inputFile(fileTitle);
	//inputFile.unsetf(ios::skipws);
	//string fileData((istream_iterator<char>(inputFile)), istream_iterator<char>());
	//content = fileData;

	return true;
}

bool CFileHandler::readFileWithQFile( const string& fileName, string& content )
{
	QString fileTitle = "source/" + QString().fromStdString(fileName);
	QFile file(fileTitle);
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
	QString fileTitle = "source/" + QString().fromStdString(fileName);
	QFile file(fileTitle);
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
	QString fileTitle = "source/" + QString().fromStdString(fileName);
	QFile file(fileTitle);
	if ( file.open(QFile::ReadOnly ) ) {
		QDataStream inStream(&file);

		char* buffer = new char[1000];
		unsigned int size;
		while(inStream.readRawData(buffer, 1000) != -1)
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
	string fileTitle = "target/" + fileName;
	QFile file(QString().fromStdString(fileTitle));
	if ( file.open(QFile::WriteOnly | QFile::Truncate) ) {
		QTextStream outStream(&file);
		outStream << QString().fromStdString(content);
	}
	file.close();

	//ofstream outputFile(fileTitle);

	return true;
}
