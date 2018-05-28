#ifndef QBOTANIO_GLOBAL_H
#define QBOTANIO_GLOBAL_H

#include <QtCore/qglobal.h>

/**
 * @author <a href="mailto:max@walter-erkrath.de">Maximilian Walter</a>
 * @since 2016-12-03 15:00
 */
#if defined(QBOTANIO_LIBRARY)
#  define QBOTANIOSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QBOTANIOSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QBOTANIO_GLOBAL_H
