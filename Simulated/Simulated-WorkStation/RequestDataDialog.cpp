
#include "RequestDataDialog.h"
#include "IceHelper.h"
#include "GlobalVariable.h"

QList<RdbRealData::RequestCompleteDataSeq> CRequestManager::m_completeDatasList;
QList<RdbRealData::RequestCompleteDataSeq> CRequestManager::m_batchCompleteDatasList;
QMutex CRequestManager::s_mutex;

CRequestDataDialog::CRequestDataDialog(RequestType requestType, QWidget* parent /*= 0*/)
	: QDialog(parent)
{
	m_requestType = requestType;
	initDialog();
}

void CRequestDataDialog::initDialog()
{
	m_listWidget = new QListWidget;	
	refreshListWidget();

	m_addButton = new QPushButton(QStringLiteral("增加请求"), this);
	connect(m_addButton, SIGNAL(clicked()), this, SLOT(addRequest()));
	m_stopButton = new QPushButton(QStringLiteral("取消请求"), this);
	connect(m_stopButton, SIGNAL(clicked()), this, SLOT(stopRequest()));

	QHBoxLayout* buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(m_addButton);
	buttonLayout->addWidget(m_stopButton);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(m_listWidget);
	layout->addLayout(buttonLayout);
	this->setLayout(layout);
}

void CRequestDataDialog::refreshListWidget()
{
	if (!m_listWidget)
	{
		return;
	}

	m_listWidget->clear();
	QList<RdbRealData::RequestCompleteDataSeq> dataList = CRequestManager::getCompleteDataSeq();
	QList<RdbRealData::RequestCompleteDataSeq>::iterator iter = dataList.begin();
	for (; iter != dataList.end(); ++iter)
	{
		QListWidgetItem* item = new QListWidgetItem(QString().fromStdString(iter->requestNode));
		m_listWidget->addItem(item);
	}
}

string g_selectItem = "";
bool isSameName(RemoteUnitItem item)
{
	return item.iedName == g_selectItem;
}

static long s_requestId = 10000;
void CRequestDataDialog::addRequest()
{
	// 查询实时库中所有的单元
	RemoteUnitItemVct dataItems;
	CIceHelper::instance()->selectRemoteUnits("", "", dataItems);

	QStringList substationItems;
	for (RemoteUnitItemVct::iterator iter = dataItems.begin(); iter != dataItems.end(); ++iter)
	{
		QString iedId = QString().fromStdString(iter->iedId);
		QString name = QString().fromStdString(iter->iedName);
		QString ecRid = QString().fromStdString(iter->ecRid);
		QString item = QString("%1 - %2 - %3").arg(iedId).arg(name).arg(ecRid);
		substationItems.push_back(name);
	}

	bool ok = false;
	QString item = QInputDialog::getItem(this, QStringLiteral("选择单元站点"), 
		QStringLiteral("选择单元站点"), substationItems, 0, false, &ok);
	if(ok && !item.isEmpty()) 
	{
		g_selectItem = item.toStdString();
		string substationId = "", iedId = "";
		RemoteUnitItemVct::iterator iter = std::find_if(dataItems.begin(), dataItems.end(), isSameName);
		if (iter != dataItems.end())
		{
			substationId = iter->ecRid;
			iedId = iter->iedId;
		}
		
		// 查看配电所下的所有开关、刀闸
		RdbRealData::RequestCompleteDataSeq dataSeq;
		dataSeq.id = 0;
		dataSeq.isStop = false;
		dataSeq.refreshFreq = 3;
		dataSeq.requestNode = CGlobalVariable::instance().getRdbRequestTopic().toStdString();
		dataSeq.requestId = s_requestId++;

		RdbDataItemVct breakerItems;
		CIceHelper::instance()->selectBreakers(ECRID, substationId, breakerItems);
		for (RdbDataItemVct::iterator iter = breakerItems.begin(); iter != breakerItems.end(); ++iter)
		{
			RdbRealData::RequestCompleteData requestData;
			requestData.tableName = BREAKER;
			requestData.fieldName = MRID;
			requestData.fieldValue = iter->mrid;
			dataSeq.seq.push_back(requestData);
		}

		RdbDataItemVct disconnectorItems;
		CIceHelper::instance()->selectDisconnectors(ECRID, substationId, disconnectorItems);
		for (RdbDataItemVct::iterator iter = disconnectorItems.begin(); 
			iter != disconnectorItems.end(); ++iter)
		{
			RdbRealData::RequestCompleteData requestData;
			requestData.tableName = DISCONNECTOR;
			requestData.fieldName = MRID;
			requestData.fieldValue = iter->mrid;
			dataSeq.seq.push_back(requestData);
		}
		dataSeq.dataCount = dataSeq.seq.size();
		CRequestManager::addCompleteDataSeq(dataSeq);

		// 查看配电所下的所有遥测、遥信
		RdbRealData::RequestCompleteDataSeq measurmentSeq;
		measurmentSeq.id = 0;
		measurmentSeq.isStop = false;
		measurmentSeq.refreshFreq = 3;
		measurmentSeq.requestNode = CGlobalVariable::instance().getRdbRequestTopic().toStdString();
		measurmentSeq.requestId = s_requestId++;

		RdbDataItemVct analogItems;
		CIceHelper::instance()->selectAnalogs(FTUUNITID, iedId, analogItems);
		for (RdbDataItemVct::iterator iter = analogItems.begin(); 
			iter != analogItems.end(); ++iter)
		{
			RdbRealData::RequestCompleteData requestData;
			requestData.tableName = ANALOG;
			requestData.fieldName = MRID;
			requestData.fieldValue = iter->mrid;

			measurmentSeq.seq.push_back(requestData);
		}
		
		RdbDataItemVct discreteItems;
		CIceHelper::instance()->selectDiscretes(FTUUNITID, iedId, discreteItems);
		for (RdbDataItemVct::iterator iter = discreteItems.begin(); 
			iter != discreteItems.end(); ++iter)
		{
			RdbRealData::RequestCompleteData requestData;
			requestData.tableName = DISCRETE;
			requestData.fieldName = MRID;
			requestData.fieldValue = iter->mrid;

			measurmentSeq.seq.push_back(requestData);
		}
		measurmentSeq.dataCount = measurmentSeq.seq.size();
		CRequestManager::addCompleteDataSeq(measurmentSeq);

		refreshListWidget();
	}
}

void CRequestDataDialog::stopRequest()
{
	QListWidgetItem* item = m_listWidget->currentItem();
	if (!item)
	{
		return;
	}
	CRequestManager::stopCompleteDataSeq(item->text().toStdString());
	refreshListWidget();
}


bool CRequestManager::s_changed = false;

QList<RdbRealData::RequestCompleteDataSeq> CRequestManager::getCompleteDataSeq()
{
	return m_completeDatasList;
}

void CRequestManager::addCompleteDataSeq(const RdbRealData::RequestCompleteDataSeq& seq)
{
	QList<RdbRealData::RequestCompleteDataSeq>::iterator iter = m_completeDatasList.begin();
	for (; iter != m_completeDatasList.end(); ++iter)
	{
		if (iter->requestNode == seq.requestNode)
		{
			return;
		}
	}
	m_completeDatasList.push_back(seq);
	setRequestChanged(true);
}

void CRequestManager::stopCompleteDataSeq(const string& requestNode)
{
	QList<RdbRealData::RequestCompleteDataSeq>::iterator iter = m_completeDatasList.begin();
	for (; iter != m_completeDatasList.end(); ++iter)
	{
		if (iter->requestNode == requestNode)
		{
			iter->isStop = true;
			setRequestChanged(true);
			return;
		}
	}
}

void CRequestManager::removeCompleteDataSeq(const string& requestNode)
{
	QList<RdbRealData::RequestCompleteDataSeq>::iterator iter = m_completeDatasList.begin();
	for (; iter != m_completeDatasList.end(); ++iter)
	{
		if (iter->requestNode == requestNode)
		{
			m_completeDatasList.erase(iter);
			return;
		}
	}
}

QList<RdbRealData::RequestCompleteDataSeq> CRequestManager::getBatchCompleteDataSeq()
{
	QMutexLocker lock(&s_mutex);
	return m_batchCompleteDatasList;
}

void CRequestManager::addBatchCompleteDataSeq(const RdbRealData::RequestCompleteDataSeq& seq)
{
	QMutexLocker lock(&s_mutex);
	m_batchCompleteDatasList.push_back(seq);
}

void CRequestManager::stopBatchCompleteDataSeq()
{
	QMutexLocker lock(&s_mutex);
	for (QList<RdbRealData::RequestCompleteDataSeq>::iterator it = m_batchCompleteDatasList.begin(); 
		it != m_batchCompleteDatasList.end(); ++it)
	{
		it->isStop = true;
	}
}

void CRequestManager::removeBatchCompleteDataSeq(long requestId)
{
	QMutexLocker lock(&s_mutex);
	QList<RdbRealData::RequestCompleteDataSeq>::iterator iter = m_batchCompleteDatasList.begin();
	for (; iter != m_batchCompleteDatasList.end(); ++iter)
	{
		if (iter->requestId == requestId)
		{
			m_batchCompleteDatasList.erase(iter);
			return;
		}
	}
}

bool CRequestManager::isRequestChanged()
{
	return s_changed;
}

void CRequestManager::setRequestChanged(bool value)
{
	s_changed = value;
}
