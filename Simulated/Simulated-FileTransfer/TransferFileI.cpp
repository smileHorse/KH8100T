
#include "TransferFileI.h"

#include <fstream>
using namespace std;

TransferFileManagerI::TransferFileManagerI()
{

}

void TransferFileManagerI::uploadFile( const ::std::string& fileName, const ::TransferFileModule::Bytes& content,
		const ::Ice::Current& /* = ::Ice::Current() */ )
{
	ofstream outf(fileName.c_str(), ios::binary | ios::app);
	if (!outf)
	{
		return;
	}

	int size = content.size();
	char* buffer = new char[size];
	memcpy(buffer, &content, size);
	outf.write(buffer, size);
	delete [] buffer;

	outf.close();
}

TransferFileModule::FileContent TransferFileManagerI::downloadFile( const ::std::string& fileName, Ice::Long pos,
		const ::Ice::Current& /* = ::Ice::Current() */ )
{
	FileContent fileContent;
	ifstream inf(fileName.c_str(), ios::binary);
	if (!inf)
	{
		fileContent.content.clear();
		return fileContent;
	}

	inf.seekg(pos);
	char buffer[FILE_BUFFER_SIZE];
	inf.read(buffer, FILE_BUFFER_SIZE);
	int nChars = inf.gcount();
	if (nChars > 0)
	{
		fileContent.pos = inf.tellg();
		fileContent.content = Bytes(buffer, buffer + nChars);
	}
	else
	{
		fileContent.content.clear();
	}

	return fileContent;
}
