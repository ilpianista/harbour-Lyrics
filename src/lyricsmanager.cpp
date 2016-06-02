/*
  The MIT License (MIT)

  Copyright (c) 2015-2016 Andrea Scarpino <me@andreascarpino.it>

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

#include "lyricsmanager.h"

#include <QCoreApplication>
#include <QDebug>
#include <QSettings>

#include "chartlyricsapi.h"
#include "geniusapi.h"
#include "lyricsmaniaapi.h"
#include "lyricswikiapi.h"
#include "provider.h"

LyricsManager::LyricsManager(QObject *parent) :
    QObject(parent)
  , api(0)
{
    settings = new QSettings(QCoreApplication::organizationDomain(), QCoreApplication::applicationName(), this);

    setProvider(settings->value("Provider").toString());
}

LyricsManager::~LyricsManager()
{
    delete settings;
    delete api;
}

QString LyricsManager::getProvider() const
{
    QString provider;

    const QString className = api->metaObject()->className();
    if (className.compare(QStringLiteral("ChartLyricsAPI")) == 0) {
        provider = "ChartLyrics";
    } else if (className.compare(QStringLiteral("GeniusAPI")) == 0) {
        provider = "Genius";
    } else if (className.compare(QStringLiteral("LyricsManiaAPI")) == 0) {
        provider = "LyricsMania";
    } else {
        provider = "LyricsWiki";
    }

    qDebug() << "Default provider is" << provider;
    return provider;
}

void LyricsManager::setProvider(const QString &provider)
{
    if (api) {
        delete api;
    }

    QString p;
    if (provider.compare(QStringLiteral("ChartLyrics")) == 0) {
        api = new ChartLyricsAPI;
        p = QStringLiteral("ChartLyrics");
    } else if (provider.compare(QStringLiteral("Genius")) == 0) {
        api = new GeniusAPI;
        p = QStringLiteral("Genius");
    } else if (provider.compare(QStringLiteral("LyricsMania")) == 0) {
        api = new LyricsManiaAPI;
        p = QStringLiteral("LyricsMania");
    } else {
        api = new LyricsWikiAPI;
        p = QStringLiteral("LyricsWiki");
    }

    qDebug() << "Setting default provider to" << p;
    settings->setValue("Provider", p);
}

void LyricsManager::search(const QString &artist, const QString &song)
{
    qDebug() << "Querying" << api->metaObject()->className();
    api->getLyric(artist, song);

    connect(api, &Provider::lyricFetched, this, &LyricsManager::searchResult);
}
