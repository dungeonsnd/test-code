#ifndef WINDOWINDLL_GLOBAL_H
#define WINDOWINDLL_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef WINDOWINDLL_LIB
# define WINDOWINDLL_EXPORT Q_DECL_EXPORT
#else
# define WINDOWINDLL_EXPORT Q_DECL_IMPORT
#endif

#endif // WINDOWINDLL_GLOBAL_H
