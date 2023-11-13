#pragma once

#include <QtCore/qglobal.h>

#if defined(GameSerializationLibrary)
#  define GameSerializationExport Q_DECL_EXPORT
#else
#  define GameSerializationExport Q_DECL_IMPORT
#endif

