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

#include "lyricswikiapi.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QWebElement>
#include <QWebFrame>
#include <QWebPage>
#include <QUrl>
#include <QUrlQuery>

#include "lyric.h"

const static QString SITE_URL = QStringLiteral("http://lyrics.wikia.com");

LyricsWikiAPI::LyricsWikiAPI(QObject *parent) :
    Provider(parent)
  , network(new QNetworkAccessManager(this))
{
}

LyricsWikiAPI::~LyricsWikiAPI()
{
    qDeleteAll(lyrics.keys());
    qDeleteAll(lyrics.values());
    delete network;
}

void LyricsWikiAPI::getLyric(const QString &artist, const QString &song)
{
    qDebug() << "Requesting lyric for artist" << artist << ", song" << song;

    QUrl url(SITE_URL);

    QString artNoSpaces(artist);
    artNoSpaces.replace(QChar::Space, QChar::fromLatin1('_'));
    QString songNoSpaces(song);
    songNoSpaces.replace(QChar::Space, QChar::fromLatin1('_'));

    url.setPath("/wiki/" + artNoSpaces + QChar::fromLatin1(':') + songNoSpaces);

    qDebug() << "Requesting lyric page" << url;
    QNetworkRequest req(url);
    QNetworkReply* reply = network->get(req);

    Lyric* lyric = new Lyric();
    lyric->setArtist(artist);
    lyric->setSong(song);
    lyrics.insert(reply, lyric);

    connect(reply, &QNetworkReply::finished, this, &LyricsWikiAPI::onGetLyricResult);
}

void LyricsWikiAPI::onGetLyricResult()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());

    bool found = false;
    Lyric* lyric = 0;

    if (reply->error() != QNetworkReply::NoError) {
        qCritical() << "Cannot fetch lyric";
    } else {
        QWebPage page;
        page.settings()->setAttribute(QWebSettings::AutoLoadImages, false);
        page.mainFrame()->setHtml(reply->readAll());
        QWebElement lyricbox = page.mainFrame()->findFirstElement("div[class=lyricbox]");

        if (lyricbox.isNull()) {
            qCritical() << "Cannot find lyric text in HTML page";
        } else {
            // Remove the <script> tags
            Q_FOREACH (QWebElement script, lyricbox.findAll(QStringLiteral("script"))) {
                script.removeFromDocument();
            }

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
