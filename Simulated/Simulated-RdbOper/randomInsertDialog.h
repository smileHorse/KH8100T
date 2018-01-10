#ifndef RANDOMINSERTDIALOG_H
#define RANDOMINSERTDIALOG_H

#include <QtWidgets/QtWidgets>

#include "randomInsertThread.h"
#include "rdbdata.h"

using namespace RdbRealData;

class RandomInsertDialog : public QDialog
{
	Q_OBJECT

public:
	RandomInsertDialog(const RdbDataOptPrx& rdbDataOptPrx, QWidget* parent = 0);

private:
	void createWidgets();
	void createLayout();
	void createConnectes();

private slots:
	void insertData();
	void stopInsertData();
	void updateProgessBar(int value);
	void updateTextEdit(const QString& text);
	void threadFinish();

private:
	QComboBox*		tableNameComboBox;
	QSpinBox*		countSpinBox;
	QProgressBar*	progressBar;
	QTextEdit*		resultTextEdit;
	QPushButton*	insertPushButton;
	QPushButton*	stopPushButton;
	
	RandomInsertThread	m_thread;

	RdbDataOptPrx	m_rdbDataOptPrx;
};

#endif