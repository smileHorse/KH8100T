
#include "transloggerI.h"
#include "FileHandler.h"

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
