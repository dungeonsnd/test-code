#include "Window1.h"
#include <QMessageBox>

Window1::Window1(const std::string & url, QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);

	_webview =ui.webView;
	_title =windowTitle();

	connect(_webview ,SIGNAL(loadProgress(int)), this, SLOT(onLoadProgress(int)));

	_webview->load( QUrl(QString::fromLocal8Bit(url.c_str())) );
}

Window1::~Window1()
{

}

void Window1::onLoadProgress ( int progress )
{
	setWindowTitle(_title+"   "+QString("%1").arg(progress)+"%");
	_bridge.sendsigLoadProgress(progress);
}

