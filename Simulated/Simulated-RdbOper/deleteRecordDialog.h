#ifndef DELETERECORDDIALOG_H
#define DELETERECORDDIALOG_H

#include <QtWidgets/QtWidgets>

#include "rdbdata.h"

using namespace RdbRealData;

class DeleteRecordDialog : public QDialog
{
	Q_OBJECT

public:
	DeleteRecordDialog(const RdbDataOptPrx& rdbDataOptPrx, QWidget* parent = 0);

private:
	void createWidgets();
	void createLayout();
	void createConnectes();
	void updateTableWidget(const RespondCompleteDataSeq& repSeq);

private slots:
	void queryRecord();
	void deleteRecords();
	void clearTableWidget();
	void tableNameChanged(const QString& tableName);

private:
	QComboBox*		tableNameComboBox;
	QComboBox*		fieldNameComboBox;
	QLineEdit*		fieldValueLineEdit;
	QTableWidget*	tableValueTableWidget;

	QPushButton*	queryButton;
	QPushButton*	deleteButton;
	QPushButton*	clearButton;

	RdbDataOptPrx	m_rdbDataOptPrx;
};
#endif