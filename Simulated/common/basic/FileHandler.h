#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include <QtCore/QtCore>

using namespace std;

class CFileHandler
{
public:
	CFileHandler();

public:
	// 打开文件
	bool openFile(const string& fileName);

	// 关闭文件
	void closeFile();

	// 读取文件
	bool readFile(const string& fileName, string& content);


	bool readFileWithFStream(const string& fileName, string& content);
	bool readFileWithQFile(const string& fileName, string& content);
	bool readFileWithQTextStream(const string& fileName, string& content);
	bool readFileWithQDataStream(const string& fileName, string& content);

	// 写入文件
	bool writeFile(const string& fileName, const string& content);

private:
	QString	getReadFilePath(const string& fileName);
	QString getWriteFilePath(const string& fileName);

};
#endif