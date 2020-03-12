
#include "FastdbManager.h"
#include "logger.h"
#include "ReadFastdbRunnable.h"
#include "ReadWriteFastdbRunnable.h"
#include "WriteFastdbRunnable.h"
#include "zthread/Thread.h"
#include "zthread/ThreadedExecutor.h"

#include <iostream>
using namespace ZThread;
using namespace std;

// 初始化log4cplus日志
log4cplus::Logger CLogger::my_logger = CLogger::init("./fastdbConcurrency.log4");

int main() {
	srand(time(NULL));

	RdbLog(CLogger::Log_INFO, "程序已启动...");
	FastdbManager* dbManager = FastdbManagerInstance::getFastdbManagerInstance();
	
	// 打开实时库
	if (!dbManager || !dbManager->openDatabase("simulated"))
	{
		RdbLog(CLogger::Log_INFO, "打开实时库失败...");
		return -1;
	}

	cout << "Press <Enter> to quit" << endl;

	CountedPtr<FastdbCollection> collection(new FastdbCollection);
	ThreadedExecutor executor;
	// 启动5个读取线程
	for (int i = 0; i < 5; ++i)
	{
		executor.execute(new ReadFastdbRunnable(i, dbManager));
	}
	// 启动10个修改线程
	for (int i = 0; i < 0; ++i)
	{
		executor.execute(new WriteFastdbRunnable(i, collection, dbManager));
	}

	// 启动10个读取修改线程
	for (int i = 0; i < 0; ++i)
	{
		executor.execute(new ReadWriteFastdbRunnable(i, collection, dbManager));
	}

	cin.get();
	executor.interrupt();
	Thread::sleep(10000);

	// 关闭实时库
	if (dbManager && dbManager->closeDatabase())
	{
		RdbLog(CLogger::Log_INFO, "已关闭实时库...");
	}

	RdbLog(CLogger::Log_INFO, "程序已退出...");
	return 0;
}