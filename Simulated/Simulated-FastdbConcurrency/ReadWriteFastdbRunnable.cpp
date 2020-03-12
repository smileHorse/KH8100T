#include "ReadWriteFastdbRunnable.h"
#include "FastdbTableDefine.h"
#include "logger.h"

void ReadWriteFastdbRunnable::run()
{
	try {
		dbDatabase* db = m_dbManager->getDbDatabase();
		while(!Thread::interrupted()) {
			// 开启fastdb事务
			if (!db->isAttached()) {
				db->attach();
			}

			// 写入数据
			int value = rand() % 3;
			if (value == 0) {
				m_fastdbHelper.insertFastdbData();
			} else if(value == 1) {
				m_fastdbHelper.updateFastdbData();
			} else {
				m_fastdbHelper.selectFastdbData();
			}

			// 关闭fastdb事务
			db->detach();
			Thread::sleep(200);
		}
	} catch (Interrupted_Exception&) {
		RdbLog(CLogger::Log_INFO, "Exit read-write thread %d by Interrupted_Exception", id);
	}
}
