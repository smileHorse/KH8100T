
#include "rdbTableFactory.h"
#include "selectCompleteDataDialog.h"
#include "FunctionExecTime.h"

#include <Ice/LocalException.h>

#define TEXT_SEPERATOR	"%"

SelectCompleteDataDialog::SelectCompleteDataDialog(const RdbDataOptPrx& rdbDataOptPrx,  QWidget* parent /*= 0*/)
	: QDialog(parent), m_rdbDataOptPrx(rdbDataOptPrx)
{
	createWidgets();
	createLayout();
	createConnectes();
}

void SelectCompleteDataDialog::createWidgets()
{
	setWindowTitle(QStringLiteral("��ѯȫ������"));
	setWindowIcon(QIcon(":/select.png"));
	resize(900, 700);

	idSpinBox = new QSpinBox();
	idSpinBox->setRange(1, 100);

	requestIdSpinBox = new QSpinBox;
	requestIdSpinBox->setRange(1, 100);

	requestNodeLineEdit = new QLineEdit;
	requestNodeLineEdit->setText("simulated-request");

	isStopCheckBox = new QCheckBox;
	isStopCheckBox->setChecked(true);

	refreshFreqSpinBox = new QSpinBox;
	refreshFreqSpinBox->setRange(0, 100);

	tableNameComboBox = new QComboBox;
	QStringList tableNames = RdbTableFactory::getTableNames();
	tableNameComboBox->addItems(tableNames);

	fieldNameComboBox = new QComboBox;
	tableNameChanged(tableNameComboBox->currentText());

	fieldValueLineEdit = new QLineEdit;

	queryButton = new QPushButton(QIcon(":/select.png"), QStringLiteral("��ѯ"));
	stopQueryButton = new QPushButton(QIcon(":/stop.png"), QStringLiteral("ֹͣ��ѯ"));
	exportButton = new QPushButton(QIcon(":/export.png"), QStringLiteral("����"));
	importButton = new QPushButton(QIcon(":/import.png"), QStringLiteral("����"));

	dataTableWidget = new QTableWidget;
	dataTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	dataTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	dataTableWidget->setSortingEnabled(true);
	dataTableWidget->setAlternatingRowColors(true);
}

void SelectCompleteDataDialog::createLayout()
{
	QFormLayout* formLayout = new QFormLayout;
	formLayout->addRow(QStringLiteral("&Id:"), idSpinBox);
	formLayout->addRow(QStringLiteral("&RequestId:"), requestIdSpinBox);
	formLayout->addRow(QStringLiteral("RequestNode:"), requestNodeLineEdit);
	formLayout->addRow(QStringLiteral("IsStop:"), isStopCheckBox);
	formLayout->addRow(QStringLiteral("RefreshFreq"), refreshFreqSpinBox);
	formLayout->addRow(QStringLiteral("TableName:"), tableNameComboBox);
	formLayout->addRow(QStringLiteral("FieldName:"), fieldNameComboBox);
	formLayout->addRow(QStringLiteral("FieldValue:"), fieldValueLineEdit);

	QHBoxLayout* buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(queryButton);
	buttonLayout->addWidget(stopQueryButton);
	buttonLayout->addWidget(exportButton);
	buttonLayout->addWidget(importButton);

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addLayout(formLayout);
	mainLayout->addLayout(buttonLayout);
	mainLayout->addWidget(dataTableWidget);

	setLayout(mainLayout);
}

void SelectCompleteDataDialog::createConnectes()
{
	connect(tableNameComboBox, SIGNAL(currentTextChanged(const QString&)), this, SLOT(tableNameChanged(const QString&)));
	connect(queryButton, SIGNAL(clicked()), this, SLOT(queryData()));
	connect(stopQueryButton, SIGNAL(clicked()), this, SLOT(stopQueryData()));
	connect(exportButton, SIGNAL(clicked()), this, SLOT(exportDatas()));
	connect(importButton, SIGNAL(clicked()), this, SLOT(importDatas()));
}

void SelectCompleteDataDialog::updateTableWidget(const RespondCompleteDataSeq& repSeq)
{
	dataTableWidget->clear();
	int rowCount = 0;
	while(rowCount = dataTableWidget->rowCount())
	{
		dataTableWidget->removeRow(rowCount - 1);
	}
	int columnCount = 0;
	while(columnCount = dataTableWidget->columnCount())
	{
		dataTableWidget->removeColumn(columnCount - 1);
	}

	QStringList headerLabels;
	headerLabels << m_currFieldNames;
	dataTableWidget->setRowCount(headerLabels.size());
	dataTableWidget->setVerticalHeaderLabels(headerLabels);

	for (size_t i = 0; i < repSeq.seq.size(); ++i)
	{
		dataTableWidget->insertColumn(i);
		for (int j = 0; j < repSeq.seq.at(i).dataValues.size(); ++j)
		{
			QTableWidgetItem* item = new QTableWidgetItem(QString().fromStdString(repSeq.seq.at(i).dataValues.at(j)));
			item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			dataTableWidget->setItem(j, i, item);
		}
	}
	dataTableWidget->resizeColumnsToContents();
}

void SelectCompleteDataDialog::tableNameChanged( const QString& tableName )
{
	fieldNameComboBox->clear();
	fieldNameComboBox->addItem("");

	m_currFieldNames.clear();
	m_currFieldNames = RdbTableFactory::getTableFields(tableName);
	if (!m_currFieldNames.isEmpty())
	{
		fieldNameComboBox->addItems(m_currFieldNames);
		fieldNameComboBox->setCurrentIndex(0);
	}
}

void SelectCompleteDataDialog::queryData()
{
	if (!m_rdbDataOptPrx)
	{
		QMessageBox::warning(this, QStringLiteral("��ѯȫ������"), QStringLiteral("��ѯʧ��: Ice����Ϊ��"));
		return;
	}

	try
	{
		// ׼����������
		RequestCompleteDataSeq reqSeq;
		reqSeq.id = idSpinBox->value();
		reqSeq.requestId = requestIdSpinBox->value();
		reqSeq.requestNode = requestNodeLineEdit->text().toStdString();
		reqSeq.isStop = isStopCheckBox->isChecked();
		reqSeq.refreshFreq = refreshFreqSpinBox->value();

		RequestCompleteData reqData;
		reqData.tableName = tableNameComboBox->currentText().toStdString();
		reqData.fieldName = fieldNameComboBox->currentText().toStdString();
		reqData.fieldValue = fieldValueLineEdit->text().toStdString();

		reqSeq.seq.push_back(reqData);
		reqSeq.dataCount = reqSeq.seq.size();

		RespondCompleteDataSeq repSeq;
		{
			CFunctionExecTime exec(__FUNCTION__, true);
			m_rdbDataOptPrx->SelectCompleteData(reqSeq, repSeq);
			QMessageBox::warning(this, QStringLiteral("��ѯȫ������"), 
				QStringLiteral("��ѯ���ݺ�ʱ: %1ms").arg(exec.ellapsed()));
		}

		updateTableWidget(repSeq);
	}
	catch(const Ice::UnknownUserException& e1) 
	{
		QMessageBox::warning(this, QStringLiteral("��ѯȫ������"), 
			QStringLiteral("��ѯ����ʧ�� UnknownUserException: %1").arg(e1.what()));
	}
	catch(const Ice::UnknownLocalException& e2)
	{
		QMessageBox::warning(this, QStringLiteral("��ѯȫ������"), 
			QStringLiteral("��ѯ����ʧ�� UnknownLocalException: %1").arg(e2.what()));
	}
	catch(const Ice::UnknownException& e3)
	{
		QMessageBox::warning(this, QStringLiteral("��ѯȫ������"), 
			QStringLiteral("��ѯ����ʧ�� UnknownException: %1").arg(e3.what()));
	}
	catch(const Ice::Exception& ex)
	{
		QMessageBox::warning(this, QStringLiteral("��ѯȫ������"), 
			QStringLiteral("��ѯ����ʧ��: %1").arg(ex.what()));
	}
}

void SelectCompleteDataDialog::stopQueryData()
{

}

void SelectCompleteDataDialog::exportDatas()
{
	QString fileName = QFileDialog::getSaveFileName(this, QStringLiteral("��������"), ".");
	if (fileName.isEmpty())
	{
		return;
	}

	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::warning(this, QStringLiteral("��������"), QStringLiteral("�ļ���ʧ��"));
		return;
	}

	QTextStream out(&file);
	out << tableNameComboBox->currentText() << "\n";

	int rowCount = dataTableWidget->rowCount();
	int colCount = dataTableWidget->columnCount();
	for (int i = 0; i < colCount; ++i)
	{
		for (int j = 0; j < rowCount; ++j)
		{
			out << (j == 0 ? "" : TEXT_SEPERATOR);
			QTableWidgetItem* item = dataTableWidget->item(j, i);
			if (item)
			{
				out << item->text();
			}
			else
			{
				out << "";
			}
		}

		out << "\n";
	}

	file.close();
	QMessageBox::information(this, QStringLiteral("��������"), QStringLiteral("�����ɹ�"));
}

void SelectCompleteDataDialog::importDatas()
{
	QString fileName = QFileDialog::getOpenFileName(this, QStringLiteral("��������"), ".");
	if (fileName.isEmpty())
	{
		return;
	}

	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::warning(this, QStringLiteral("��������"), QStringLiteral("�ļ���ʧ��"));
		return;
	}

	QTextStream in(&file);

	// ��ȡ����
	QString tableName = in.readLine();
	if(tableName.isNull())
	{
		return;
	}

	// ��ȡ����
	QString line = in.readLine();
	QList<QStringList>	datas;
	while(!line.isNull())
	{
		QStringList strs = line.split(TEXT_SEPERATOR);
		datas.push_back(strs);
		line = in.readLine();
	}

	if (!m_rdbDataOptPrx)
	{
		QMessageBox::warning(this, QStringLiteral("��������"), QStringLiteral("����ʧ��: Ice����Ϊ��"));
		return;
	}

	try
	{
		RespondCompleteDataSeq repSeq;

		foreach(QStringList strs, datas)
		{
			RespondCompleteData data;
			data.tableName = tableName.toStdString();
			foreach(QString str, strs)
			{
				data.dataValues.push_back(str.toStdString());
			}

			repSeq.seq.push_back(data);
		}
		
		RespondCompleteDataSequence repSequence;
		bool ret = m_rdbDataOptPrx->InsertData(repSeq, repSequence);
		if (!ret)
		{
			QMessageBox::warning(this, QStringLiteral("��������"), 
				QStringLiteral("��������ʧ��"));
		}
		else
		{
			QMessageBox::warning(this, QStringLiteral("��������"), 
				QStringLiteral("�������ݳɹ�"));
		}
	}
	catch(const Ice::Exception& ex)
	{
		QMessageBox::warning(this, QStringLiteral("��������"), 
			QStringLiteral("��������ʧ��: %1").arg(ex.what()));
	}
}
