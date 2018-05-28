#include "stdafx.h"

//Botanuio URL-Shortener service URL
static const QString BOTANIO_URL_SHORTENER_SERVICE_URL      = QStringLiteral("https://api.botan.io/s/");
//Botanio URL-Shortener URL fragments names
static const QString BOTANIO_URL_SHORTENER_TOKEN_FRAGMENT_NAME      = QStringLiteral("token");
static const QString BOTANIO_URL_SHORTENER_URL_FRAGMENT_NAME        = QStringLiteral("url");
static const QString BOTANIO_URL_SHORTENER_USERID_FRAGMENT_NAME     = QStringLiteral("user_ids");

//------------------------------------------------------------------------------------
//@author  max walter @date 06.12.2016
QBotanioUrlShortenerAPIPrivate::QBotanioUrlShortenerAPIPrivate(QObject *parent)
    : QObject(parent)
{
    m_TimeoutTimer.setSingleShot(true);

    connect(&m_TimeoutTimer , &QTimer::timeout, [this](){
        if(m_pReply && m_pReply->isRunning()){
            m_pReply->close();
        }
    });

}
//------------------------------------------------------------------------------------
//@author  max walter @date 06.12.2016
QBotanioUrlShortenerAPIPrivate::~QBotanioUrlShortenerAPIPrivate()
{

}
//------------------------------------------------------------------------------------
//@author  max walter @date 06.12.2016
bool QBotanioUrlShortenerAPIPrivate::shortenUrlAsynch(const QString& sApiKey, int UID, const QString& sUrl, int timeoutMs)
{
    bool bResult = false;
    if (!m_pReply)
    {
        //setup URL
        QString sFullUrl = QString("%1?%2=%3&%4=%5&%6=%7")
                            .arg(BOTANIO_URL_SHORTENER_SERVICE_URL)
                            .arg(BOTANIO_URL_SHORTENER_TOKEN_FRAGMENT_NAME)
                            .arg(sApiKey)
                            .arg(BOTANIO_URL_SHORTENER_URL_FRAGMENT_NAME)
                            .arg(sUrl)
                            .arg(BOTANIO_URL_SHORTENER_USERID_FRAGMENT_NAME)
                            .arg(QString::number(UID));

        //stack request together
        QNetworkRequest request(sFullUrl);
        //POST request
        m_pReply = m_NAM.get(request);
        if (m_pReply)
        {
            connect(m_pReply, SIGNAL(finished()), this, SLOT(slotRequestFinished()));
            bResult = true;
            //timeout
            if(timeoutMs > 0)
                m_TimeoutTimer.start(timeoutMs);
        }
    }
    return bResult;
}
//------------------------------------------------------------------------------------
//@author  max walter @date 06.12.2016
QString QBotanioUrlShortenerAPIPrivate::shortenUrl(const QString &sApiKey, int UID, const QString &sUrlToBeShorten, bool &bResult, QString &sResultDetails, int timeoutMs)
{
    QString sResultUrl = sUrlToBeShorten;
    bResult = shortenUrlAsynch(sApiKey, UID, sUrlToBeShorten, timeoutMs);
    if(bResult)
    {
        //Lokal "event loop" instance
        QEventLoop loop;
        QObject::connect(this, &QBotanioUrlShortenerAPIPrivate::signalFinished, &loop, &QEventLoop::quit);
        connect(this, &QBotanioUrlShortenerAPIPrivate::signalFinished,[this,&sResultUrl,&sResultDetails,&bResult](bool result, const QString& sShortenURL,const QString& sResult){
            bResult = result;
            if(bResult)
                sResultUrl = sShortenURL;
            sResultDetails = sResult;
        });
        loop.exec();
    }
    return sResultUrl;
}
//------------------------------------------------------------------------------------
//@author  max walter @date 06.12.2016
void QBotanioUrlShortenerAPIPrivate::slotRequestFinished()
{
    //stop timeout timer
    m_TimeoutTimer.stop();
    if (m_pReply)
    {
        QString sShortenURL;
        QString sResultDetails;
        bool bResult = false;
        QNetworkReply::NetworkError error = m_pReply->error();
        if (error == QNetworkReply::NoError)
        {
            sShortenURL = m_pReply->readAll();
            bResult = true;
        }
        else
            sResultDetails = tr("http request failed! %1").arg(m_pReply->errorString());

        m_pReply->deleteLater();
        m_pReply = nullptr;
        //inform client
        emit signalFinished(bResult, sShortenURL, sResultDetails);
        deleteLater();
    }
}
