#ifndef CONFIGICEDIALOG_H
#define CONFIGICEDIALOG_H

#include <QtWidgets/QtWidgets>

struct ConfigIceInfo
{
	QString	iceIp;
	int icePort;
	QString	iceOffsideIp;
	int iceOffsidePort;
	QString iceStormIp;
	int iceStormPort;
};

class ConfigIceDialog : public QDialog
{
	Q_OBJECT

public:
	ConfigIceDialog(const QIcon& icon = QIcon(":/config.png"), QWidget* parent = 0);

private:
	void createDialogs();
	void createLayout();

public:
	ConfigIceInfo	getConfigIceInfo() const;

private:
	QIcon	m_icon;

	QLabel* iceIpLabel;
	QLabel* icePortLabel;
	QLabel* iceOffsideIpLabel;
	QLabel* iceOffsidePortLabel;
	QLabel*	iceStormIpLabel;
	QLabel*	iceStormPortLabel;

	QLineEdit*	iceIpLineEdit;
	QSpinBox*	icePortSpinBox;
	QLineEdit*	iceOffsideIpLineEdit;
	QSpinBox*	iceOffsidePortSpinBox;
	QLineEdit*	iceStormIpLineEdit;
	QSpinBox*	iceStormPortSpinBox;

	QDialogButtonBox*	buttonBox;
};
#endif