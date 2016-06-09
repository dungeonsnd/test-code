#include "webview_test.h"

webview_test::webview_test(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);

	ui.webView->load( QUrl(QString::fromLocal8Bit("http://www.126.com")) );
}

webview_test::~webview_test()
{

}
