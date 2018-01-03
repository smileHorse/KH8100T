
#include "configIceDialog.h"

ConfigIceDialog::ConfigIceDialog(const QIcon& icon, QWidget* parent /*= 0*/ )
	: QDialog(parent), m_icon(icon)
{
	createDialogs();
	createLayout();
}

void ConfigIceDialog::createDialogs()
{
	iceIpLineEdit = new QLineEdit("192.168.3.25");
	icePortSpinBox = new QSpinBox;
	icePortSpinBox->setRange(10000, 11000);
	iceOffsideIpLineEdit = new QLineEdit("192.168.3.25");
	iceOffsidePortSpinBox = new QSpinBox;
	iceOffsidePortSpinBox->setRange(10000, 11000);
	iceStormIpLineEdit = new QLineEdit("192.168.3.226");
	iceStormPortSpinBox = new QSpinBox;
	iceStormPortSpinBox->setRange(10000, 11000);

	iceIpLabel = new QLabel(QStringLiteral("Ice服务IP:"));
	iceIpLabel->setBuddy(iceIpLineEdit);
	icePortLabel = new QLabel(QStringLiteral("Ice服务端口:"));
	icePortLabel->setBuddy(icePortSpinBox);
	iceOffsideIpLabel = new QLabel(QStringLiteral("对侧Ice服务IP:"));
	iceOffsideIpLabel->setBuddy(iceIpLineEdit);
	iceOffsidePortLabel = new QLabel(QStringLiteral("对侧Ice服务端口:"));
	iceOffsidePortLabel->setBuddy(icePortSpinBox);
	iceStormIpLabel = new QLabel(QStringLiteral("IceStorm服务IP:"));
	iceStormIpLabel->setBuddy(iceStormIpLineEdit);
	iceStormPortLabel = new QLabel(QStringLiteral("IceStorm服务端口:"));
	iceStormPortLabel->setBuddy(iceStormPortSpinBox);

	buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	setWindowIcon(m_icon);
}

void ConfigIceDialog::createLayout()
{
	QGridLayout* layout = new QGridLayout;
	layout->addWidget(iceIpLabel, 0, 0);
	layout->addWidget(iceIpLineEdit, 0, 1);
	layout->addWidget(icePortLabel, 1, 0);
	layout->addWidget(icePortSpinBox, 1, 1);
	layout->addWidget(iceOffsideIpLabel, 2, 0);
	layout->addWidget(iceOffsideIpLineEdit, 2, 1);
	layout->addWidget(iceOffsidePortLabel, 3, 0);
	layout->addWidget(iceOffsidePortSpinBox, 3, 1);
	layout->addWidget(iceStormIpLabel, 4, 0);
	layout->addWidget(iceStormIpLineEdit, 4, 1);
	layout->addWidget(iceStormPortLabel, 5, 0);
	layout->addWidget(iceStormPortSpinBox, 5, 1);
	layout->addWidget(buttonBox, 6, 0, 1, 2);
	setLayout(layout);
}

ConfigIceInfo ConfigIceDialog::getConfigIceInfo() const
{
	ConfigIceInfo info;
	info.iceIp = iceIpLineEdit->text();
	info.icePort = icePortSpinBox->value();
	info.iceOffsideIp = iceOffsideIpLineEdit->text();
	info.iceOffsidePort = iceOffsidePortSpinBox->value();
	info.iceStormIp = iceStormIpLineEdit->text();
	info.iceStormPort = iceStormPortSpinBox->value();
	return info;
}
