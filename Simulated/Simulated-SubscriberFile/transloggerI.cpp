
#include "transloggerI.h"
#include "FileHandler.h"

#include <fstream>
using namespace std;

 
#define BUFFER_SIZE 2048 * 2

TransferRdbLoggerInfoI::TransferRdbLoggerInfoI()
{

}

void TransferRdbLoggerInfoI::transferRdbLogger( const ::std::string& title, const ::std::string& data, const ::Ice::Current& /* = ::Ice::Current() */ )
{
	CFileHandler fileHandler;
	if (fileHandler.writeFile(title, data))
	{
	}
}

void TransferRdbLoggerInfoI::transferRdbLoggerBinary( const ::std::string& title, const ::TransferRdbLogger::Bytes& datas, 
	const ::Ice::Current& /* = ::Ice::Current() */ )
{
	ofstream fout("target/" + title, std::ios::binary | std::ios::app);

	if (!datas.empty())
	{
		char szBuf[BUFFER_SIZE] = {0};  
		unsigned size = datas.size();
		for (int i = 0; i < size; ++i)
		{
			szBuf[i] = datas[i];
		}
		fout.write(szBuf, sizeof(char) * size);
	}
}
