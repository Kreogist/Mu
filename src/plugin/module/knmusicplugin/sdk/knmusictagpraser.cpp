#include "knmusictagpraser.h"

KNMusicTagParser::KNMusicTagParser(QObject *parent) :
    QObject(parent)
{

}

void KNMusicTagParser::setTextData(QString &destination, const QString &source)
{
    if(!source.isEmpty())
    {
        destination=source;
    }
}
