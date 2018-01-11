
#include "updateRecordDialog.h"
#include "rdbTableFactory.h"

UpdateRecordDialog::UpdateRecordDialog(const RdbDataOptPrx& rdbDataOptPrx, QWidget* parent /*= 0*/ )
	: QDialog(parent), m_rdbDataOptPrx(rdbDataOptPrx)
{
	createWidgets();
	createLayout();
	createConnectes();
}

void UpdateRecordDialog::createWidgets()
{
	setWindowTitle(QStringLiteral("更新记录"));
	setWindowIcon(QIcon(":/updateRecord.png"));
	resize(600, 600);

	tableNameComboBox = new QComboBox;
	QStringList tableNames = RdbTableFactory::getTableNames();
	tableNameComboBox->addItems(tableNames);
	if (!tableNames.isEmpty())
	{
		tableNameComboBox->setCurrentIndex(0);
	}

	fieldNameComboBox = new QComboBox;
	fieldValueLineEdit = new QLineEdit;

	tableValueTableWidget = new QTableWidget;
	tableValueTableWidget->setAlternatingRowColors(true);
	tableValueTableWidget->setMinimumSize(400, 600);

	tableNameChanged(tableNameComboBox->currentText());

	queryButton = new QPushButton(QIcon(":/select.png"), QStringLiteral("查询记录"));
	updateButton = new QPushButton(QIcon(":/updateRecord.png"), QStringLiteral("更新记录"));
	clearButton = new QPushButton(QIcon(":/clear.png"), QStringLiteral("清空结果表"));
}

void UpdateRecordDialog::createLayout()
{
	QFormLayout* formLayout = new QFormLayout;
	formLayout->addRow(QStringLiteral("表名:"), tableNameComboBox);
	formLayout->addRow(QStringLiteral("查询列名:"), fieldNameComboBox);
	formLayout->addRow(QStringLiteral("查询列值:"), fieldValueLineEdit);
	formLayout->addRow(QStringLiteral("表值:"), tableValueTableWidget);

	QHBoxLayout* buttonLayout = new QHBoxLayout;
	buttonLayout->addStretch(1);
	buttonLayout->addWidget(queryButton);
	buttonLayout->addWidget(updateButton);
	buttonLayout->addWidget(clearButton);

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addLayout(formLayout);
	mainLayout->addLayout(buttonLayout);
	setLayout(mainLayout);
}

void UpdateRecordDialog::createConnectes()
{
	connect(tableNameComboBox, SIGNAL(currentTextChanged(const QString&)), this, SLOT(tableNameChanged(const QString&)));
	connect(queryButton, SIGNAL(clicked()), this, SLOT(queryRecord()));
	connect(updateButton, SIGNAL(clicked()), this, SLOT(updateRecords()));
	connect(clearButton, SIGNAL(clicked()), this, SLOT(clearTableWidget()));
}

void UpdateRecordDialog::updateTableWidget( const RespondCompleteDataSeq& repSeq )
{
	clearTableWidget();
	if (repSeq.seq.empty())
	{
		QMessageBox::warning(this, QStringLiteral("查询结果"), QStringLiteral("返回结果为空"));
		return;
	}

	int rowCount = tableValueTableWidget->rowCount();
	for (size_t i = 0; i < repSeq.seq.size(); ++i)
	{
		tableValueTableWidget->insertColumn(i);
		int dataCount = repSeq.seq.at(i).dataValues.size();
		if (rowCount != dataCount)
		{
			QMessageBox::warning(this, QStringLiteral("查询结果"), 
				QStringLiteral("返回结果个数错误: 所需个数(%1) 返回个数(%2)").arg(rowCount).arg(dataCount));
			return;
		}

		for (int j = 0; j < dataCount; ++j)
		{
			QTableWidgetItem* item = new QTableWidgetItem(QString().fromStdString(repSeq.seq.at(i).dataValues.at(j)));
			item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			tableValueTableWidget->setItem(j, i, item);
		}
	}	
}

void UpdateRecordDialog::queryRecord()
{
	if (!m_rdbDataOptPrx)
	{
		QMessageBox::warning(this, QStringLiteral("查询记录"), QStringLiteral("查询失败: Ice代理为空"));
		return;
	}

	try
	{
		RequestCompleteDataSeq reqSeq;
		reqSeq.id = 1;
		reqSeq.requestId = 1;
		reqSeq.requestNode = "udpateRecord";
		reqSeq.isStop = true;
		reqSeq.refreshFreq = 0;
		
		RequestCompleteData data;
		data.tableName = tableNameComboBox->currentText().toStdString();
		data.fieldName = fieldNameComboBox->currentText().toStdString();
		data.fieldValue = fieldValueLineEdit->text().toStdString();

		reqSeq.seq.push_back(data);
		reqSeq.dataCount = reqSeq.seq.size();

		RespondCompleteDataSeq repSeq;
		bool ret = m_rdbDataOptPrx->SelectCompleteData(reqSeq, repSeq);
		if (!ret)
		{
			QMessageBox::warning(this, QStringLiteral("查询记录"), 
				QStringLiteral("查询记录失败"));
		}
		else
		{
			updateTableWidget(repSeq);
		}
	}
	catch(const Ice::Exception& ex)
	{
		QMessageBox::warning(this, QStringLiteral("查询记录"), 
			QStringLiteral("查询记录失败: %1").arg(ex.what()));
	}
}

void UpdateRecordDialog::updateRecords()
{
	if (!m_rdbDataOptPrx)
	{
		QMessageBox::warning(this, QStringLiteral("更新数据"), QStringLiteral("更新失败: Ice代理为空"));
		return;
	}

	try
	{
		RespondCompleteDataSeq repSeq;

		int rowCount = tableValueTableWidget->rowCount();
		int columnCount = tableValueTableWidget->columnCount();
		for (int col = 0; col < columnCount; ++col)
		{
			RespondCompleteData data;
			data.tableName = tableNameComboBox->currentText().toStdString();
			for (int row = 0; row < rowCount; ++row)
			{
				QTableWidgetItem* item = tableValueTableWidget->item(row, col);
				if (item)
				{
					data.dataValues.push_back(item->text().toStdString());
					if (row == 0)
					{
						data.dataRid = item->text().toStdString();
					}
				}
				else
				{
					data.dataValues.push_back("");
				}
			}

			repSeq.seq.push_back(data);
		}
		repSeq.dataCount = repSeq.seq.size();

		RespondCompleteDataSequence repSequence;
		bool ret = m_rdbDataOptPrx->UpdateCompleteData(repSeq, repSequence);
		if (!ret)
		{
			QMessageBox::warning(this, QStringLiteral("更新数据"), 
				QStringLiteral("更新数据失败"));
		}
		else
		{
			QMessageBox::warning(this, QStringLiteral("更新数据"), 
				QStringLiteral("更新数据成功"));
		}
	}
	catch(const Ice::Exception& ex)
	{
		QMessageBox::warning(this, QStringLiteral("更新数据"), 
			QStringLiteral("更新数据失败: %1").arg(ex.what()));
	}
}

void UpdateRecordDialog::clearTableWidget()
{
	tableValueTableWidget->clear();
	int rowCount = 0;
	while(rowCount = tableValueTableWidget->rowCount())
	{
		tableValueTableWidget->removeRow(rowCount - 1);
	}
	int columnCount = 0;
	while(columnCount = tableValueTableWidget->columnCount())
	{
		tableValueTableWidget->removeColumn(columnCount - 1);
	}

	QString tableName = tableNameComboBox->currentText();
	QStringList tableFields = RdbTableFactory::getTableFields(tableName);
	tableValueTableWidget->setRowCount(tableFields.size());
	tableValueTableWidget->setVerticalHeaderLabels(tableFields);
}

void UpdateRecordDialog::tableNameChanged( const QString& tableName )
{
	fieldNameComboBox->clear();
	QStringList tableFields = RdbTableFactory::getTableFields(tableName);
	fieldNameComboBox->addItems(tableFields);
	if (!tableFields.isEmpty())
	{
		fieldNameComboBox->setCurrentIndex(0);
	}
	
	clearTableWidget();
}
