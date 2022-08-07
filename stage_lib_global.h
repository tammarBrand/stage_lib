#ifndef STAGE_LIB_GLOBAL_H
#define STAGE_LIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(STAGE_LIB_LIBRARY)
#  define STAGE_LIB_EXPORT Q_DECL_EXPORT
#else
#  define STAGE_LIB_EXPORT Q_DECL_IMPORT
#endif

#endif // STAGE_LIB_GLOBAL_H
