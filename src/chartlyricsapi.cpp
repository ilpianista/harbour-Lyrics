/*
  The MIT License (MIT)

  Copyright (c) 2015 Andrea Scarpino <me@andreascarpino.it>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include "chartlyricsapi.h"

#include <QDebug>
#include <QDomDocument>
#include <QDomNode>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>

#include "lyric.h"

const static QString API_URL = QStringLiteral("http://api.chartlyrics.com/apiv1.asmx/SearchLyricDirect");

ChartLyricsAPI::ChartLyricsAPI(QObject *parent) :
    Provider(parent)
  , network(new QNetworkAccessManager(this))
{
}

ChartLyricsAPI::~ChartLyricsAPI()
{
    delete network;
}

void ChartLyricsAPI::getLyric(const QString &artist, const QString &song)
{
    qDebug() << "Requesting lyric for artist" << artist << ", song" << song;
    QUrl url(API_URL);

    QUrlQuery query;
    query.addQueryItem(QStringLiteral("artist"), artist);
    query.addQueryItem(QStringLiteral("song"), song);
    url.setQuery(query);

    QNetworkRequest req(url);
    QNetworkReply* reply = network->get(req);

    connect(reply, &QNetworkReply::finished, this, &ChartLyricsAPI::onGetLyricResult);
}

void ChartLyricsAPI::onGetLyricResult()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());

    bool found = false;
    Lyric* lyric = 0;

    if (reply->error() != QNetworkReply::NoError) {
        qCritical() << "Cannot fetch lyric";
    } else {
        QDomDocument xml;
        QString errorMsg;
        xml.setContent(reply->readAll(), &errorMsg);
        if (errorMsg.isEmpty()) {
            QDomNodeList results = xml.elementsByTagName(QStringLiteral("GetLyricResult"));
            if (!results.isEmpty()) {
                QDomNode res = results.at(0);

                lyric = new Lyric();
                lyric->setArtist(res.namedItem(QStringLiteral("LyricArtist")).toElement().text());
                lyric->setSong(res.namedItem(QStringLiteral("LyricSong")).toElement().text());

                const QString text = res.namedItem(QStringLiteral("Lyric")).toElement().text();
                if (!text.isEmpty()) {
                    lyric->setText(text);
                    found = true;
                } else {
                    qDebug() << "Got an empty text";
                }
            } else {
                qDebug() << "No lyric found";
            }
        } else {
            qCritical() << "Got an invalid XML:" << errorMsg;
        }
    }

    Q_EMIT lyricFetched(lyric, found);

    reply->deleteLater();
}
