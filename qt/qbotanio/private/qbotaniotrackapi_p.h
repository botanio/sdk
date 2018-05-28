#ifndef QBOTANIOTRACKAPIPRIVATE_H
#define QBOTANIOTRACKAPIPRIVATE_H

#include <QObject>
#include <QJsonDocument>
#include <QVariantMap>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QNetworkReply>
#include <QPointer>

class QBotanioTrackApiPrivate : public QObject
{
    Q_OBJECT
public:
    QBotanioTrackApiPrivate(QObject *parent);
    ~QBotanioTrackApiPrivate();
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
protected:
    bool parseResult(QByteArray jsonResult, QString &sResult);
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
#endif // QBOTANIOTRACKAPIPRIVATE_H
