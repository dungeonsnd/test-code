#ifndef TESTAPPLICATION_H
#define TESTAPPLICATION_H

#include <QtGui/QDialog>
#include "ui_window1.h"
#include "windowindll_bridge.h"

class Window1 : public QDialog
{
	Q_OBJECT

public:
	Window1(const std::string & url, QWidget *parent = 0, Qt::WFlags flags = 0);
	~Window1();
	
	WindowInDllBridge * bridge()
	{
		return &_bridge;
	}

public slots:
	void onLoadProgress (int progress);

private:
	Ui::Window1 ui;
	QWebView * _webview;
	QString _title;
	WindowInDllBridge _bridge;
};

#endif // TESTAPPLICATION_H
