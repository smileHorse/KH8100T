#ifndef TRANSFERFILE_COMMON_H
#define TRANSFERFILE_COMMON_H

#include <QtCore/QString>

// 文件传输方式
enum TransferMode
{
	Transfer_Ice,
	Transfer_IceStorm
};

// 文件读写格式
enum FileOperMode
{
	FileOper_Text,
	FileOper_Binary
};

// 文件操作接口
enum FileOperInterface
{
	FileInter_C,
	FileInter_CPlusPlus
};

// 传输服务服务端适配器名称
extern const QString serverAdapterIdentify;

// 传输服务客户端适配器名称
extern const QString clientAdapterIdentify;

// 传输服务客户端代理接口名称
extern const QString clientProxyIdentify;

// 每次传输的缓存大小
#define TransferBufferSize	4096

#endif