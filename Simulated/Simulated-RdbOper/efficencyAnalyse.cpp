
#include <QtWidgets/QtWidgets>

#include "efficencyAnalyse.h"
#include "logger.h"

EfficiencyAnalyseThread::EfficiencyAnalyseThread( const QString& fileName, QObject* parent )
	: QThread(parent), _fileName(fileName)
{

}

// 开始分析文件
void EfficiencyAnalyseThread::startAnalyse()
{
	QFile file(_fileName);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		QMessageBox::warning(0, QStringLiteral("打开文件"), 
							QStringLiteral("打开文件失败: %1").arg(file.errorString()));
		return;
	}

	QTextStream textStream(&file);
	QString line;
	int lineNumber = 0;			// 文件的总行数
	QDateTime prevDateTime;
	QDateTime nextDateTime;

	SecondData  secondData;
	MinuteData	minuteData;
	HourData	hourData;
	while(textStream.readLineInto(&line))
	{
		// 第一行时仅转换时间值
		if (lineNumber == 0)
		{
			// 获取第一行的时间
			if (!transferStringToDateTime(line, prevDateTime))
			{
				continue;
			}
			secondData.second = prevDateTime.time().second();
			secondData.count++;

			minuteData.minute = prevDateTime.time().minute();
			hourData.hour = prevDateTime.time().hour();

			_startDateTime = prevDateTime;
			++lineNumber;
		}
		else
		{
			// 获取当前行的时间
			if (!transferStringToDateTime(line, nextDateTime))
			{
				continue;
			}

			if (prevDateTime.time().second() == nextDateTime.time().second())
			{
			}
			else if (prevDateTime.time().minute() == nextDateTime.time().minute())
			{
				minuteData.secondDatas.push_back(secondData);
				secondData.clear();
			}
			else if (prevDateTime.time().hour() == nextDateTime.time().hour())
			{
				minuteData.secondDatas.push_back(secondData);
				secondData.clear();
			
				hourData.minuteDatas.push_back(minuteData);
				minuteData.clear();
				minuteData.minute = nextDateTime.time().minute();
			}
			else
			{
				minuteData.secondDatas.push_back(secondData);
				secondData.clear();

				hourData.minuteDatas.push_back(minuteData);
				minuteData.clear();

				_hourDatas.push_back(hourData);
				hourData.clear();

				minuteData.minute = nextDateTime.time().minute();
				hourData.hour = nextDateTime.time().hour();
			}

			secondData.second = nextDateTime.time().second();
			secondData.count++;

			prevDateTime = nextDateTime;
			++lineNumber;
		}
	}

	minuteData.secondDatas.push_back(secondData);
	hourData.minuteDatas.push_back(minuteData);
	_hourDatas.push_back(hourData);

	_endDateTime = nextDateTime;

	QString analyseInfo;
	generateAnalyseInfo(analyseInfo);
	outputAnalyseInfo(analyseInfo);

	file.close();
}

void EfficiencyAnalyseThread::run()
{
	startAnalyse();
}

// 转换字符串为QDateTime
bool EfficiencyAnalyseThread::transferStringToDateTime( const QString& text, QDateTime& dateTime )
{
	QStringList textes = text.split(" ");
	if (textes.size() < 2)
	{
		return false;
	}

	QString date = textes[0];
	QStringList dateList = date.split('-');
	if (dateList.size() != 3)
	{
		return false;
	}
	int year = dateList[0].toInt();
	int month = dateList[1].toInt();
	int day = dateList[2].toInt();
	QDate qDate(year, month, day);

	QString time = textes[1];
	QStringList timeList = time.split(':');
	if (timeList.size() != 3)
	{
		return false;
	}
	int hour = timeList[0].toInt();
	int minute = timeList[1].toInt();
	int second = timeList[2].toInt();
	QTime qTime(hour, minute, second);

	dateTime = QDateTime(qDate, qTime);
	return true;
}

// 产生分析信息
void EfficiencyAnalyseThread::generateAnalyseInfo(QString& analyseInfo)
{
	int executeSecond;
	QString executeTime;
	computeExecuteTime(executeSecond, executeTime);

	QString executeEfficiency;
	computeExecuteEfficiency(executeEfficiency);

	analyseInfo += QStringLiteral("总行数: %1, 运行时间: %2, 平均效率: %3")
			.arg(computeDataCount())
			.arg(executeTime)
			.arg(executeEfficiency);

	foreach(const HourData& data, _hourDatas)
	{
		analyseInfo += "\n";
		analyseInfo += QStringLiteral("小时: %1, 执行次数: %2, 平均执行次数: %3次/秒")
			.arg(data.hour).arg(data.count()).arg(data.average());

		foreach(const MinuteData& minuteData, data.minuteDatas)
		{
			analyseInfo += "\n\t";
			analyseInfo += QStringLiteral("分钟: %1, 执行次数: %2, 平均执行次数: %3次/秒")
				.arg(minuteData.minute).arg(minuteData.count()).arg(minuteData.average());

			foreach(const SecondData& secondData, minuteData.secondDatas)
			{
				analyseInfo += "\n\t\t";
				analyseInfo += QStringLiteral("秒: %1, 执行次数: %2, 平均执行次数: %3次/秒")
					.arg(secondData.second).arg(secondData.sum()).arg(secondData.average());
			}
		}
	}

}

void EfficiencyAnalyseThread::outputAnalyseInfo(const QString& analyseInfo)
{
	QFile file("./log/efficencyLog.log");
	if (!file.open(QFile::WriteOnly | QFile::Text))
	{
		return;
	}

	QTextStream textStream(&file);
	textStream << analyseInfo;

	file.close();
}

// 计算记录行数
long EfficiencyAnalyseThread::computeDataCount()
{
	long sum = 0;
	foreach(const HourData& hourData, _hourDatas)
	{
		sum += hourData.count();
	}
	return sum;
}

// 计算运行时间
void EfficiencyAnalyseThread::computeExecuteTime(int& executeSec,  QString& executeTime )
{
	executeSec = _startDateTime.secsTo(_endDateTime);
	int executeHour = executeSec / 3600;
	int executeMinute = (executeSec % 3600) / 60;
	int executeSecond = (executeSec % 3600) % 60;
	executeTime = QStringLiteral("%1小时 %2分 %3秒").arg(executeHour).arg(executeMinute).arg(executeSecond);
}

// 计算平均效率
void EfficiencyAnalyseThread::computeExecuteEfficiency( QString& executeEfficiency )
{
	int executeSecond;
	QString executeTime;
	computeExecuteTime(executeSecond, executeTime);

	if (executeSecond > 0)
	{
		executeEfficiency = QStringLiteral("%1 次/秒").arg(computeDataCount() / executeSecond);
	}
	else
	{
		executeEfficiency = QStringLiteral("&&& 次/秒");
	}
}
