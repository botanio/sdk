#include "stdafx.h"

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//@author  max walter @date 05.12.2016
QBotanioTrackApi *QBotanioTrackApi::newInstance()
{
    return new QBotanioTrackApi();
}
//------------------------------------------------------------------------------------
//@author  max walter @date 05.12.2016
QBotanioTrackApi::QBotanioTrackApi()
    : QObject(nullptr), d(new QBotanioTrackApiPrivate(this))
{
    connect(d.data(), SIGNAL(signalFinished(bool, const QString&)),
            this, SIGNAL(signalFinished(bool, const QString&)));
}
//------------------------------------------------------------------------------------
//@author  max walter @date 05.12.2016
QBotanioTrackApi::~QBotanioTrackApi()
{

}
//------------------------------------------------------------------------------------
//@author  max walter @date 05.12.2016
bool QBotanioTrackApi::track(const QString& sApiKey, int UID, const QString& sEventName, const QJsonDocument& params, int timeoutMs)
{    
    return d->track(sApiKey, UID, sEventName, params, timeoutMs);
}
//------------------------------------------------------------------------------------
//@author  max walter @date 06.12.2016
bool QBotanioTrackApi::track(const QString &sApiKey, int UID, const QString &sEventName, int timeoutMs)
{
    return d->track(sApiKey, UID, sEventName,timeoutMs);
}
//------------------------------------------------------------------------------------
//@author  max walter @date 05.12.2016
bool QBotanioTrackApi::track(const QString &sApiKey, int UID, const QString &sEventName, const QVariantMap &params, int timeoutMs)
{
    return d->track(sApiKey, UID, sEventName,params,timeoutMs);
}
