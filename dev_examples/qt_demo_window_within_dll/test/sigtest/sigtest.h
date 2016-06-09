#ifndef SIGTEST_H
#define SIGTEST_H

#include <QtGui/QMainWindow>
#include "ui_sigtest.h"

class T : public QObject
{
Q_OBJECT
	
signals:
	void sigT(int progress);

public:
	T(){};
	void sendsigT(int progress)
	{
		emit /*this->*/sigT(progress);
	}
};


class sigtest : public QMainWindow
{
	Q_OBJECT

public:
	sigtest(QWidget *parent = 0, Qt::WFlags flags = 0);
	~sigtest();

private:
	Ui::sigtestClass ui;
};

#endif // SIGTEST_H
