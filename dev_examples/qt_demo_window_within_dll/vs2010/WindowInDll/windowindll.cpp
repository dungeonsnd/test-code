#include "windowindll.h"
#include "window1.h"
#include "imp.h"

WindowInDll::WindowInDll(const std::string & url)
{
	_imp =new Imp(url);
}

WindowInDll::~WindowInDll()
{
	delete _imp;
}

WindowInDllBridge * WindowInDll::bridge()
{
	if(_imp)
		return _imp->bridge();
	else
		return NULL;
}

