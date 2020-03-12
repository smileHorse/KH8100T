#ifndef REQUESTDATADIALOG_H
#define REQUESTDATADIALOG_H

#include <QtWidgets/QtWidgets>
#include <string>
using namespace std;

#include "rdbdata.h"

class QListWidget;
class QPushButton;

//请求类型
enum RequestType
{
	DefaultData, SpecificData, CompleteData, TopoData
};

// 记录系统中所有的请求
class CRequestManager
{
public:
	static QList<RdbRealData::RequestCompleteDataSeq> getCompleteDataSeq();
	static void addCompleteDataSeq(const RdbRealData::RequestCompleteDataSeq& seq);
	static void stopCompleteDataSeq(const string& requestNode);
	static void removeCompleteDataSeq(const string& requestNode);

	static QList<RdbRealData::RequestCompleteDataSeq> getBatchCompleteDataSeq();
	static void addBatchCompleteDataSeq(const RdbRealData::RequestCompleteDataSeq& seq);
	static void stopBatchCompleteDataSeq();
	static void removeBatchCompleteDataSeq(long requestId);

	static bool isRequestChanged();
	static void setRequestChanged(bool value);
private:
	static QList<RdbRealData::RequestCompleteDataSeq> m_completeDatasList;
	static QList<RdbRealData::RequestCompleteDataSeq> m_batchCompleteDatasList;
	static bool s_changed;

	static QMutex s_mutex;
};


class CRequestDataDialog : public QDialog
{
	Q_OBJECT

public:
	CRequestDataDialog(RequestType requestType, QWidget* parent = 0);

private:
	void initDialog();
	void refreshListWidget();

private slots:
	void addRequest();
	void stopRequest();

private:
	QListWidget* m_listWidget;
	QPushButton* m_addButton;
	QPushButton* m_stopButton;

	RequestType m_requestType;
};

#endif