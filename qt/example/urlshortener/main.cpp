#include <QCoreApplication>
#include <QDebug>

//QBotanioAPI
#include <qbotaniourlshortenerapi.h>

//Insert here your "botan.io" application key
static const QString BOTANIO_APP_KEY = QStringLiteral("YOUR_PRIVATE_KEY");
static const int UNIQUE_SET_ID       = 1234;//Telegram user id
static const QString URL             = QStringLiteral("https://github.com/botanio/sdk");

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //create a new "Botanio URL Shortener API" instance on the heap
    QBotanioUrlShortenerAPI* pBotaino = QBotanioUrlShortenerAPI::newInstance();

    //prints result after "shorten URL" request is finished
    QObject::connect(pBotaino,&QBotanioUrlShortenerAPI::signalFinished,[](bool bResult, const QString& sShortenURL,const QString& sResultDetails){
        qDebug()<<"result" << (bResult? sShortenURL : sResultDetails);
    });

    //close application after "shorten URL" request is finished
    QObject::connect(pBotaino,&QBotanioUrlShortenerAPI::signalFinished,&a,&QCoreApplication::quit);

    //call asynchronously "shortenURL" request
    pBotaino->shortenUrlAsynch(BOTANIO_APP_KEY, UNIQUE_SET_ID, URL);
    //client don't need to delete the pBotaino instance, because it is self destructed after HTTP request is finished
    //delete pBotaino;
    return a.exec();
}
