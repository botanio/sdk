#ifndef QBOTANIOURLSHORTENERAPIPRIVATE_H
#define QBOTANIOURLSHORTENERAPIPRIVATE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QNetworkReply>
#include <QPointer>

class QBotanioUrlShortenerAPIPrivate : public QObject
{
    Q_OBJECT
public:
    QBotanioUrlShortenerAPIPrivate(QObject *parent = 0);
    ~QBotanioUrlShortenerAPIPrivate();
public:
    //get shorten URL synchronously
    QString shortenUrl(const QString& sApiKey, int UID, const QString& sUrltobeShorten, bool& bResult, QString& sResultDetails, int timeoutMs = 30000);
    //get shorten URL asynchronously
    bool shortenUrlAsynch(const QString& sApiKey, int UID, const QString& sUrl,int timeoutMs = 30000);
signals:
    //Informs clients about request is finished
    void signalFinished(bool bResult, const QString& sShortenURL,const QString& sResultDetails);
protected slots:
    void slotRequestFinished();
private:
    //Network access manager
    QNetworkAccessManager m_NAM;
    //reply for current request
    QPointer<QNetworkReply> m_pReply = nullptr;
    //timeout timer
    QTimer m_TimeoutTimer;
};


#endif // QBOTANIOURLSHORTENERAPIPRIVATE_H
