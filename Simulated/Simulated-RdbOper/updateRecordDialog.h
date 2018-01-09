#ifndef UPDATERECORDDIALOG_H
#define UPDATERECORDDIALOG_H

#include <QtWidgets/QtWidgets>

#include "rdbdata.h"

using namespace RdbRealData;

class UpdateRecordDialog : public QDialog
{
	Q_OBJECT

public:
	UpdateRecordDialog(const RdbDataOptPrx& rdbDataOptPrx, QWidget* parent = 0);

private:
	void createWidgets();
	void createLayout();
	void createConnectes();
	void updateTableWidget(const RespondCompleteDataSeq& repSeq);

private slots:
	void queryRecord();
	void updateRecords();
	void clearTableWidget();
	void tableNameChanged(const QString& tableName);

private:
	QComboBox*		tableNameComboBox;
	QComboBox*		fieldNameComboBox;
	QLineEdit*		fieldValueLineEdit;
	QTableWidget*	tableValueTableWidget;

	QPushButton*	queryButton;
	QPushButton*	updateButton;
	QPushButton*	clearButton;

	RdbDataOptPrx	m_rdbDataOptPrx;
};
#endif