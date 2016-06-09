#include "sigtest.h"
#include<QObject>
/*
class T : public QObject
{
Q_OBJECT
	
signals:
	void sigT(int progress);

public:
	T(){};
	void sendsigT(int progress)
	{
		emit sigT(progress);
	}
};
*/


sigtest::sigtest(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	
	T t;
	t.sendsigT(100);
}

sigtest::~sigtest()
{

}
