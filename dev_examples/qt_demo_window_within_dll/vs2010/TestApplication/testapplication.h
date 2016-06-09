#ifndef TESTAPPLICATION_H
#define TESTAPPLICATION_H

#include <QtGui/QMainWindow>
#include "ui_testapplication.h"

class WindowInDll;

class TestApplication : public QMainWindow
{
	Q_OBJECT

public:
	TestApplication(QWidget *parent = 0, Qt::WFlags flags = 0);
	~TestApplication();
	
public slots:
	void onLoadProgress (int progress);

private:
	Ui::TestApplicationClass ui;
	QString _title;

	WindowInDll * _dll;
};

#endif // TESTAPPLICATION_H
