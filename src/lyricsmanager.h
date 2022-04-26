/*
  The MIT License (MIT)

  Copyright (c) 2015-2022 Andrea Scarpino <andrea@scarpino.dev>

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

#ifndef LYRICSMANAGER_H
#define LYRICSMANAGER_H

#include <QObject>
#include <QString>

#include "lyric.h"

class QSettings;
class Provider;

class LyricsManager : public QObject
{
    Q_OBJECT
public:
    explicit LyricsManager(QObject *parent = 0);
    virtual ~LyricsManager();

    Q_INVOKABLE void clearCache();
    Q_INVOKABLE QString getProvider() const;
    Q_INVOKABLE void search(const QString &artist, const QString &song);
    Q_INVOKABLE bool getMediaPlayerScanner() const;
    Q_INVOKABLE void setMediaPlayerScanner(const bool enabled);
    Q_INVOKABLE void setProvider(const QString &provider);

Q_SIGNALS:
    void searchResult(Lyric *lyric, const bool &found);

private:
    QString getLyricsDir() const;
    void storeLyric(Lyric *lyric, const bool &found);

    QSettings *settings;
    Provider *api;

};

#endif // LYRICSMANAGER_H
