#include "testapplication.h"
#include <QObject>
#include <QMessageBox>
#include "windowindll.h"
#include "windowindll_bridge.h"


TestApplication::TestApplication(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags), _dll(NULL)
{
	ui.setupUi(this);
	
	_title =windowTitle();

	_dll =new WindowInDll("http://www.126.com");
	WindowInDllBridge * bridge =_dll->bridge();

	bool rt =connect( bridge , SIGNAL(sigLoadProgress(int)), this, SLOT(onLoadProgress(int)) );
	if(false==rt)
	{
		QMessageBox::warning(NULL,"TestApplication","failed");
	}
	
}

TestApplication::~TestApplication()
{
	delete _dll;
	_dll =NULL;
}
        
void TestApplication::onLoadProgress ( int progress )
{
	setWindowTitle(_title+"    е§дкМгди:"+QString("%1").arg(progress)+"%");
}
