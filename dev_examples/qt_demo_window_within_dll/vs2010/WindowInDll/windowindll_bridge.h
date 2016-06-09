#ifndef _HEADER_FILE_WINDOWINDLL_BRIDGE_H_
#define _HEADER_FILE_WINDOWINDLL_BRIDGE_H_

#include <qobject.h>

class WindowInDllBridge : public QObject
{
Q_OBJECT

Q_SIGNALS:
	void sigLoadProgress(int progress);

public:
	void sendsigLoadProgress(int progress)
	{
		emit this->sigLoadProgress(progress);
	}
};

#endif // _HEADER_FILE_WINDOWINDLL_BRIDGE_H_
