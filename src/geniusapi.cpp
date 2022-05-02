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

#include "geniusapi.h"
#include "geniusapi_secret.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QWebElement>
#include <QWebFrame>
#include <QWebPage>

#include "lyric.h"

const static QString API_URL = QStringLiteral("https://api.genius.com");

GeniusAPI::GeniusAPI(QObject *parent) :
    Provider(parent)
  , network(new QNetworkAccessManager(this))
{
}

GeniusAPI::~GeniusAPI()
{
    qDeleteAll(lyrics.keyBegin(), lyrics.keyEnd());
    qDeleteAll(lyrics);
    delete network;
}

void GeniusAPI::getLyric(const QString &artist, const QString &song)
{
    qDebug() << "Requesting lyric for artist" << artist << ", song" << song;

    QUrl url(API_URL + "/search");

    QUrlQuery query;
    query.addQueryItem(QStringLiteral("q"), artist + QChar::Space + song);
    url.setQuery(query);

    QNetworkRequest req(url);

    if (CLIENT_ID[0] == '\0') {
        qCritical() << "No client id set, the request will be rejected!";
    }
    req.setRawHeader(QByteArray("client_id"), QByteArray(CLIENT_ID));

    if (CLIENT_SECRET[0] == '\0') {
        qCritical() << "No client secret set, the request will be rejected!";
    }
    req.setRawHeader(QByteArray("client_secret"), QByteArray(CLIENT_SECRET));

    if (CLIENT_ACCESS_TOKEN[0] == '\0') {
        qCritical() << "No client access token set, the request will be rejected!";
    }
    req.setRawHeader(QByteArray("Authorization"), QStringLiteral("Bearer %1").arg(CLIENT_ACCESS_TOKEN).toLatin1());

    QNetworkReply* reply = network->get(req);

    connect(reply, &QNetworkReply::finished, this, &GeniusAPI::onGetLyricResult);
}

void GeniusAPI::onGetLyricResult()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());

    bool err = true;

    if (reply->error() != QNetworkReply::NoError) {
        qCritical() << "Cannot fetch lyric:" << reply->errorString();
    } else {
        QJsonDocument json = QJsonDocument::fromJson(reply->readAll());

        if (!json.isNull()) {
            QJsonValue response = json.object().value("response");

            if (!response.isNull()) {
                QJsonArray hits = response.toObject().value("hits").toArray();

                if (!hits.isEmpty()) {
                    // 'We'are lucky' like search
                    QJsonObject result = hits.at(0).toObject().value("result").toObject();

                    Lyric* lyric = new Lyric();
                    lyric->setArtist(result.value("primary_artist").toObject().value("name").toString());
                    lyric->setSong(result.value("title").toString());

                    const QUrl url(result.value("url").toString());
                    getLyricText(url, lyric);

                    err = false;
                } else {
                    qDebug() << "No results";
                }
            } else {
                qDebug() << "No results";
            }
        } else {
            qCritical() << "Got an invalid JSON!";
        }
    }

    if (err) {
        Q_EMIT lyricFetched(0, !err);
    }

    reply->deleteLater();
}

void GeniusAPI::onGetLyricPageResult()
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

        QWebElement lyricbox = page.mainFrame()->findFirstElement("div[class=lyrics]");

        if (lyricbox.isNull()) {
            qCritical() << "Cannot find lyric text in HTML page";
        } else {
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

void GeniusAPI::getLyricText(const QUrl &url, Lyric *lyric)
{
    qDebug() << "Requesting lyric page" << url.url();
    QNetworkRequest req(url);
    QNetworkReply* reply = network->get(req);
    lyrics.insert(reply, lyric);

    connect(reply, &QNetworkReply::finished, this, &GeniusAPI::onGetLyricPageResult);
}
