
#include "TransferFileItem.h"

// 计算文件大小
QString TransferFileItem::calcFileSize( long value )
{
	if (value == 0)
	{
		return QStringLiteral("0 KB");
	}
	else 
	{
		long val = value / 1024;
		return QStringLiteral("%1 KB").arg(val);
	}
}

// 转换时间
QString TransferFileItem::convertTime( long msec )
{
	if (msec < 1000)
	{
		return QStringLiteral("%1 ms").arg(msec);
	}
	else
	{
		long sec = msec / 1000;
		msec = msec % 1000;
		if (sec < 60)
		{
			return QStringLiteral("%1.%2 ms").arg(sec).arg(msec);
		}
		else
		{
			long min = sec / 60;
			sec = sec % 60;
			if (min < 60)
			{
				return QStringLiteral("%1:%2.%3 ms").arg(min).arg(sec).arg(msec);
			}
			else
			{
				long hour = min / 60;
				min = min % 60;
				return QStringLiteral("%1:%2:%3.%4 ms").arg(hour).arg(min).arg(sec).arg(msec);
			}
		}
	}
}
