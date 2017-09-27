#include "hidecolumndialog.h"

HideColumnDialog::HideColumnDialog( QTableWidget* tableWidget, QWidget* parent /*= 0*/ )
	: QDialog(parent), m_tableWidget(tableWidget)
{
	hideColumns.clear();

	createWidget();
	initColumnWidget();
}

QList<int> HideColumnDialog::getHideColumns()
{
	return hideColumns;
}

void HideColumnDialog::createWidget()
{
	columnWidget = new QListWidget();
	columnWidget->setSelectionMode(QAbstractItemView::MultiSelection);
	columnWidget->setSelectionRectVisible(true);
	columnWidget->setAlternatingRowColors(true);
	
	okBtn = new QPushButton("确定");
	connect(okBtn, SIGNAL(clicked()), this, SLOT(setHideColumns()));

	cancelBtn = new QPushButton("取消");
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(close()));

	QHBoxLayout* btnLayout = new QHBoxLayout;
	btnLayout->addWidget(okBtn);
	btnLayout->addWidget(cancelBtn);

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addWidget(columnWidget, 4);
	mainLayout->addLayout(btnLayout, 1);

	setLayout(mainLayout);

	setWindowTitle("隐藏列");
	setWindowIcon(QIcon(":/images/hideColumn.png"));
}

void HideColumnDialog::initColumnWidget()
{
	if (!m_tableWidget)
	{
		return;
	}

	int columnCount = m_tableWidget->columnCount();
	for (int i = 0; i < columnCount; ++i)
	{
		QTableWidgetItem* item = m_tableWidget->horizontalHeaderItem(i);
		columnWidget->insertItem(i, item->text());
		QListWidgetItem* columnItem = columnWidget->item(i);
		if (m_tableWidget->isColumnHidden(i))
		{
			columnWidget->setItemSelected(columnItem, true);
		}
		else
		{
			columnWidget->setItemSelected(columnItem, false);
		}
	}
}

void HideColumnDialog::setHideColumns()
{
	hideColumns.clear();
	int count = columnWidget->count();
	for (int i = 0; i < count; ++i)
	{
		QListWidgetItem* item = columnWidget->item(i);
		if(columnWidget->isItemSelected(item))
		{
			hideColumns.push_back(i);
		}
	}
	
	accept();
}
