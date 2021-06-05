/*
  The MIT License (MIT)

  Copyright (c) 2015-2021 Andrea Scarpino <andrea@scarpino.dev>

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

#include "lyricsmaniaapi.h"

#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QWebPage>
#include <QWebElement>
#include <QWebFrame>

#include "lyric.h"

const static QString BASE_URL = QStringLiteral("http://www.lyricsmania.com");

LyricsManiaAPI::LyricsManiaAPI(QObject *parent) :
    Provider(parent)
  , network(new QNetworkAccessManager(this))
{
}

LyricsManiaAPI::~LyricsManiaAPI()
{
    qDeleteAll(lyrics.keys());
    qDeleteAll(lyrics.values());
    delete network;
}

void LyricsManiaAPI::getLyric(const QString &artist, const QString &song)
{
    qDebug() << "Requesting lyric for artist" << artist << ", song" << song;

    QString artNoSpaces(artist.toLower());
    artNoSpaces.replace(QChar::Space, QChar::fromLatin1('_'));
    QString songNoSpaces(song.toLower());
    songNoSpaces.replace(QChar::Space, QChar::fromLatin1('_'));

    QUrl url(BASE_URL + QStringLiteral("/%1_lyrics_%2.html").arg(songNoSpaces).arg(artNoSpaces));
    qDebug() << url;

    QNetworkRequest req(url);
    QNetworkReply* reply = network->get(req);

    Lyric* lyric = new Lyric();
    lyric->setArtist(artist);
    lyric->setSong(song);
    lyrics.insert(reply, lyric);

    connect(reply, &QNetworkReply::finished, this, &LyricsManiaAPI::onGetLyricPageResult);
}

void LyricsManiaAPI::onGetLyricPageResult()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());

    bool found = false;
    Lyric* lyric = 0;

    if (reply->error() != QNetworkReply::NoError) {
        qCritical() << "Cannot fetch lyric";
    } else {
        QWebPage page;
        page.settings()->setAttribute(QWebSettings::AutoLoadImages, false);
        page.settings()->setAttribute(QWebSettings::JavascriptEnabled, false);
        page.mainFrame()->setHtml(reply->readAll());

        QWebElement lyricbox = page.mainFrame()->findFirstElement("div[class=lyrics-body]");

        if (lyricbox.isNull()) {
            qCritical() << "Cannot find lyric text in HTML page";
        } else {
            // Remove the video div
            lyricbox.findFirst(QStringLiteral("div")).removeFromDocument();
            // Remove the song title
            lyricbox.findFirst(QStringLiteral("strong")).removeFromDocument();

            lyric = lyrics.take(reply);

            if (!lyric) {
                qCritical() << "Got an invalid lyric object!";
            } else {
                lyric->setText(lyricbox.toPlainText());

                found = true;
            }
        }
    }

    qDebug() << "Lyric found:" << found;
    Q_EMIT lyricFetched(lyric, found);

    reply->deleteLater();
}
