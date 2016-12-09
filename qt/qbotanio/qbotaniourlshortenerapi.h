#ifndef QBOTANIOURLSHORTENERAPI_H
#define QBOTANIOURLSHORTENERAPI_H
#include "qbotanio_global.h"

#include <QObject>
/**
 * Simple (a)synchronous "shorten URL" client implementation with Qt5.5 for Botan.io https://botan.io
 *
 * @author <a href="mailto:max@walter-erkrath.de">Maximilian Walter</a>
 * @since 2016-12-03 15:00
 */
class QBotanioUrlShortenerAPIPrivate;
class QBOTANIOSHARED_EXPORT QBotanioUrlShortenerAPI : public QObject
{
    Q_OBJECT
public:
    static QBotanioUrlShortenerAPI * newInstance();
public:
    //get shorten URL synchronously
    QString shortenUrl(const QString& sApiKey, int UID, const QString& sUrlToBeShorten, bool& bResult, QString& sResultDetails, int timeoutMs = 30000);
    //get shorten URL asynchronously
    bool shortenUrlAsynch(const QString& sApiKey, int UID, const QString& sUrl,int timeoutMs = 30000);
signals:
    //Informs clients about request is finished
    void signalFinished(bool bResult, const QString& sShortenURL,const QString& sResultDetails);
private:
    //shouldn't be called from clients. Use static QBotanioUrlShortenerAPI * newInstance();
    QBotanioUrlShortenerAPI();
    ~QBotanioUrlShortenerAPI();
    //PIMPL
    const QScopedPointer<QBotanioUrlShortenerAPIPrivate> d;
};

#endif // QBOTANIOURLSHORTENERAPI_H
