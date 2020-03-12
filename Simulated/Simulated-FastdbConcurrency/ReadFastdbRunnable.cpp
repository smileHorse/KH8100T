
#include "ReadFastdbRunnable.h"
#include "logger.h"
#include "FastdbManager.h"

void ReadFastdbRunnable::run() {
	try {
		dbDatabase* db = m_dbManager->getDbDatabase();
		while(!Thread::interrupted()) {
			// 开启fastdb事务
			if (!db->isAttached())
			{
				db->attach();
			}

			// 查询数据
			m_fastdbHelper.selectFastdbData();

			// 关闭fastdb事务
			db->detach();
			Thread::sleep(200);
		}
	} catch(Interrupted_Exception&) {
		RdbLog(CLogger::Log_INFO, "Exit read thread %d by Interrupted_Exception", id);
	}
}

