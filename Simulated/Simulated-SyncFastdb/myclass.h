#ifndef MYCLASS_H
#define MYCLASS_H

#include <QtWidgets/QMainWindow>

class MyClass : public QMainWindow
{
	Q_OBJECT

public:
	MyClass(QWidget *parent = 0);
	~MyClass();

private:
};

#endif // MYCLASS_H
