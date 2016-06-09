#ifndef WINDOWINDLL_H
#define WINDOWINDLL_H

#include "windowindll_global.h"
#include <string>

class WindowInDllBridge;
class Imp;

class WINDOWINDLL_EXPORT WindowInDll
{
public:
	WindowInDll(const std::string & url);
	~WindowInDll();

	WindowInDllBridge * bridge();
public:
	Imp * _imp;
};

#endif // WINDOWINDLL_H
