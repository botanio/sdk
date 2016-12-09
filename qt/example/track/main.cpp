#include <QCoreApplication>
#include <QDebug>

//QBotanioAPI
#include <qbotaniotrackapi.h>

//Insert here your "botan.io" application key
static const QString BOTANIO_APP_KEY    = QStringLiteral("SOME_PRIVATE_KEY");
static const int UNIQUE_SET_ID          = 1234;//e.g. telegram user id
static const QString EVENT_NAME         = QStringLiteral("TEST_EVENT");

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //create a new "Botanio track API" instance on the heap
    QBotanioTrackApi* pBotaino = QBotanioTrackApi::newInstance();

    //prints result after "track" request is finished
    QObject::connect(pBotaino,&QBotanioTrackApi::signalFinished,[](bool bResult, const QString& sResultDetails){
        Q_UNUSED(bResult);qDebug()<<"result" << sResultDetails;
    });

    //close application after "track" request is finished
    QObject::connect(pBotaino,&QBotanioTrackApi::signalFinished,&a,&QCoreApplication::quit);

    //some metrics
    QVariantMap params;
    params["int_metric"] = 1234;
    params["double_metric"] = 12.34;
    params["string_metric"] = "1234";
    params["bool_metric"] = true;
    //call asynchronously "track" request
    pBotaino->track(BOTANIO_APP_KEY, UNIQUE_SET_ID, EVENT_NAME, params);
    //client don't neede to delete the pBotaino instance, because it is self destructed after HTTP request is finished
    //delete pBotaino;

    return a.exec();
}
