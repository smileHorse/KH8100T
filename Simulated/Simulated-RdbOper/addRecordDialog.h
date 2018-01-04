#ifndef ADDRECORDDIALOG_H
#define ADDRECORDDIALOG_H

#include <QtWidgets/QtWidgets>

#include "rdbdata.h"

using namespace RdbRealData;

class AddRecordDialog : public QDialog
{
	Q_OBJECT

public:
	AddRecordDialog(const RdbDataOptPrx& rdbDataOptPrx, QWidget* parent = 0);

private:
	void createWidgets();
	void createLayout();
	void createConnectes();

private slots:
	void addRecord();
	void saveRecords();
	void updateTableWidget(const QString& tableName);

private:
	QComboBox*		tableNameComboBox;
	QTableWidget*	tableValueTableWidget;

	QPushButton*	addButton;
	QPushButton*	saveButton;

	RdbDataOptPrx	m_rdbDataOptPrx;
};
#endif