#include "webview_test.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	webview_test w;
	w.show();
	return a.exec();
}
