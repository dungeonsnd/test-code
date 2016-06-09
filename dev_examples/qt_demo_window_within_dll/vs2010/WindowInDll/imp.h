#ifndef _HEADER_FILE_IMP_H_
#define _HEADER_FILE_IMP_H_

#include <string>

class Window1;
class WindowInDllBridge;

class Imp
{
public:
	Imp(const std::string & url)
	{
		_window1 =new Window1(url);
		_window1->show();
	}
	~Imp()
	{
		delete _window1;
	}

	WindowInDllBridge * bridge()
	{
		return _window1->bridge();
	}

private:
	Window1 * _window1;
};

#endif // _HEADER_FILE_IMP_H_
