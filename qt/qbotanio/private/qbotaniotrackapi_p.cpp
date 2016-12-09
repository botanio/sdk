#include "stdafx.h"

//Botanio API base URL
static const QString BOTANIO_BASE_URL               = QStringLiteral("https://api.botan.io/track");
//Botanio base URL fragments names
static const QString BOTANIO_BASE_TOKEN_FRAGMENT_NAME    = QStringLiteral("token");
static const QString BOTANIO_BASE_UID_FRAGMENT_NAME      = QStringLiteral("uid");
static const QString BOTANIO_BASE_NAME_FRAGMENT_NAME     = QStringLiteral("name");

//------------------------------------------------------------------------------------
//@author  max walter @date 09.12.2016
QBotanioTrackApiPrivate::QBotanioTrackApiPrivate(QObject *parent)
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
QBotanioTrackApiPrivate::~QBotanioTrackApiPrivate()
{

}
//------------------------------------------------------------------------------------
//@author  max walter @date 06.12.2016
bool QBotanioTrackApiPrivate::track(const QString& sApiKey, int UID, const QString& sEventName, const QJsonDocument& params, int timeoutMs)
{
    bool bResult = false;
    if (!m_pReply)
    {
        //setup URL
        QString sFullUrl = QString("%1?%2=%3&%4=%5&%6=%7")
                            .arg(BOTANIO_BASE_URL)
                            .arg(BOTANIO_BASE_TOKEN_FRAGMENT_NAME)
                            .arg(sApiKey)
                            .arg(BOTANIO_BASE_UID_FRAGMENT_NAME)
                            .arg(QString::number(UID))
                            .arg(BOTANIO_BASE_NAME_FRAGMENT_NAME)
                            .arg(sEventName);

        //pack JSON params
        QByteArray ba = params.toJson();
        QNetworkRequest request(sFullUrl);
        request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(QStringLiteral("application/json")));
        request.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(ba.size()));

        //POST request
        m_pReply = m_NAM.post(request, ba);
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
bool QBotanioTrackApiPrivate::track(const QString &sApiKey, int UID, const QString &sEventName, int timeoutMs)
{
    return track(sApiKey, UID, sEventName,QJsonDocument(),timeoutMs);
}
//------------------------------------------------------------------------------------
//@author  max walter @date 06.12.2016
bool QBotanioTrackApiPrivate::track(const QString &sApiKey, int UID, const QString &sEventName, const QVariantMap &params, int timeoutMs)
{
    return track(sApiKey, UID, sEventName,QJsonDocument(QJsonObject::fromVariantMap(params)),timeoutMs);
}

//------------------------------------------------------------------------------------
//@author  max walter @date 06.12.2016
bool QBotanioTrackApiPrivate::parseResult(QByteArray jsonResult, QString& sResult)
{
    bool bResult = false;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonResult);
    if (!jsonDoc.isNull() && jsonDoc.isObject())
    {
        QJsonObject obj = jsonDoc.object();
        if (obj.contains("status") )
        {
            //Status available?
            QString sStatus = obj.value("status").toString();
            if(sStatus == QStringLiteral("accepted"))
            {
                sResult = sStatus;
                bResult = true;
            }
            else
                if(sStatus == QStringLiteral("failed") || sStatus == QStringLiteral("bad request")){
                    sResult = sStatus;
                    bResult = false;
                }

            //Info availabvle?
            if(obj.contains("info"))
                sResult.append(QStringLiteral(" %1").arg(obj.value("info").toString()));
        }
        else
            sResult = tr("Invalid json structure");
    }
    else
        sResult = tr("Invalid json document");

    return bResult;
}
//------------------------------------------------------------------------------------
//@author  max walter @date 06.12.2016
void QBotanioTrackApiPrivate::slotRequestFinished()
{
    //stop timeout timer
    m_TimeoutTimer.stop();
    if (m_pReply)
    {
        bool bResult = false;
        QNetworkReply::NetworkError error = m_pReply->error();
        QString sResultDetails;
        if (error == QNetworkReply::NoError)
        {
            QByteArray baResult = m_pReply->readAll();
            bResult = parseResult(baResult, sResultDetails);
        }
        else
            sResultDetails = tr("http request failed! %1").arg(m_pReply->errorString());

        m_pReply->deleteLater();
        m_pReply = nullptr;
        //inform client
        emit signalFinished(bResult, sResultDetails);
        deleteLater();
    }
}
