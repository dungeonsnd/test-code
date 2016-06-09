#ifndef WEBVIEW_TEST_H
#define WEBVIEW_TEST_H

#include <QtGui/QDialog>
#include "ui_webview_test.h"

class webview_test : public QDialog
{
	Q_OBJECT

public:
	webview_test(QWidget *parent = 0, Qt::WFlags flags = 0);
	~webview_test();

private:
	Ui::webview_testClass ui;
};

#endif // WEBVIEW_TEST_H
