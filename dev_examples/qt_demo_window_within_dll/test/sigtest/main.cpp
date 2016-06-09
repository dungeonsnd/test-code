#include "sigtest.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	sigtest w;
	w.show();
	return a.exec();
}
