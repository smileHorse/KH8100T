
#include "WriteFastdbRunnable.h"
#include "FastdbTableDefine.h"
#include "logger.h"

void WriteFastdbRunnable::run()
{
	try {
		dbDatabase* db = m_dbManager->getDbDatabase();
		while(!Thread::interrupted()) {
			// 开启fastdb事务
			if (!db->isAttached()) {
				db->attach();
			}

			// 写入数据
			if (rand() % 2 == 0) {
				m_fastdbHelper.insertFastdbData();
			} else {
				m_fastdbHelper.updateFastdbData();
			}

			// 关闭fastdb事务
			db->detach();
			Thread::sleep(200);
		}
	} catch (Interrupted_Exception&) {
		RdbLog(CLogger::Log_INFO, "Exit write thread %d by Interrupted_Exception", id);
	}
}


