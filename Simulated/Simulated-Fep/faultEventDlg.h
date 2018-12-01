#ifndef FAULTEVENTDLG_H
#define FAULTEVENTDLG_H

#include <QtWidgets/QDialog>

class QLabel;
class QSpinBox;
class QPushButton;
class FepServerThread;

class CFaultEventDlg : public QDialog
{
	Q_OBJECT

public:
	CFaultEventDlg(FepServerThread*	fepServerThreadPtr, QWidget* parent = 0);

private:
	void createWidgets();
	QSpinBox* createSpinBox(int max, int min);
	void createConnections();

private slots:
	void saveConfigData();

signals:
	void sendFaultEvent(int unitNo, int moduleNo, int moduleType, int infoNo, int state);

private:
	QLabel* unitNoLabel;
	QLabel* moduleNoLabel;
	QLabel* moduleTypeLabel;
	QLabel* infoNoLabel;
	QLabel* stateLabel;

	QSpinBox* unitNoSpinBox;
	QSpinBox* moduleNoSpinBox;
	QSpinBox* moduleTypeSpinBox;
	QSpinBox* infoNoSpinBox;
	QSpinBox* stateSpinBox;

	QPushButton* okButton;
	QPushButton* closeButton;
	
	FepServerThread*	m_fepServerThreadPtr;
};
#endif
