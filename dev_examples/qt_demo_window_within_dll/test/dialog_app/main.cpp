#include "dialog_app.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	dialog_app w;
	w.show();
	return a.exec();
}
