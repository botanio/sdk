#include "stdafx.h"

//------------------------------------------------------------------------------------
//@author  max walter @date 06.12.2016
QBotanioUrlShortenerAPI *QBotanioUrlShortenerAPI::newInstance()
{
    return new QBotanioUrlShortenerAPI();
}
//------------------------------------------------------------------------------------
//@author  max walter @date 06.12.2016
QBotanioUrlShortenerAPI::QBotanioUrlShortenerAPI()
    : QObject(nullptr), d(new QBotanioUrlShortenerAPIPrivate(this))
{

    connect(d.data(), SIGNAL(signalFinished(bool, const QString&,const QString&)),
            this, SIGNAL(signalFinished(bool, const QString&,const QString&)));
}
//------------------------------------------------------------------------------------
//@author  max walter @date 06.12.2016
QBotanioUrlShortenerAPI::~QBotanioUrlShortenerAPI()
{

}

//------------------------------------------------------------------------------------
//@author  max walter @date 06.12.2016
QString QBotanioUrlShortenerAPI::shortenUrl(const QString &sApiKey, int UID, const QString &sUrlToBeShorten, bool &bResult, QString &sResultDetails, int timeoutMs)
{
    return d->shortenUrl(sApiKey, UID, sUrlToBeShorten, bResult, sResultDetails, timeoutMs);
}
//------------------------------------------------------------------------------------
//@author  max walter @date 06.12.2016
bool QBotanioUrlShortenerAPI::shortenUrlAsynch(const QString& sApiKey, int UID, const QString& sUrl, int timeoutMs)
{
   return d->shortenUrlAsynch(sApiKey, UID, sUrl, timeoutMs);
}
