#ifndef QBOTANIOAPI_H
#define QBOTANIOAPI_H
#include "qbotanio_global.h"

#include <QObject>

/**
 * Simple asynchronous "track" client implementation with Qt5.5 for Botan.io https://botan.io

 * @author <a href="mailto:max@walter-erkrath.de">Maximilian Walter</a>
 * @since 2016-12-03 15:00
 */
class QBotanioTrackApiPrivate;
class QBOTANIOSHARED_EXPORT QBotanioTrackApi : public QObject
{
    Q_OBJECT
public:
    static QBotanioTrackApi * newInstance();
public://method
    //track event
    bool track(const QString& sApiKey, int UID, const QString& sEventName,int timeoutMs = 30000);
    //track event with params as varaint map
    bool track(const QString& sApiKey, int UID, const QString& sEventName,const QVariantMap& params,int timeoutMs = 30000);
    //track event with params as json document
    bool track(const QString& sApiKey, int UID, const QString& sEventName,const QJsonDocument& params,int timeoutMs = 30000);
signals:
    //Informs clients about request is finished
    void signalFinished(bool bResult, const QString& sResultDetails);
private :
    //shouldn't be called from clients. Use static QBotanioTrackApi * newInstance();
    QBotanioTrackApi();
    ~QBotanioTrackApi();
    //PIMPL
    const QScopedPointer<QBotanioTrackApiPrivate> d;
};

#endif // QBOTANIOAPI_H
