/*
  The MIT License (MIT)

  Copyright (c) 2022 Andrea Scarpino <andrea@scarpino.dev>

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

#ifndef AZLYRICS_H
#define AZLYRICS_H

#include <QMap>
#include <QString>

#include "provider.h"

class QNetworkAccessManager;
class QNetworkReply;

class AZLyrics : public Provider
{
    Q_OBJECT
    Q_INTERFACES(Provider)
public:
    explicit AZLyrics(QObject *parent = 0);
    virtual ~AZLyrics();

    void getLyric(const QString &artist, const QString &song);

private:
    void onGetLyricPageResult();

    QNetworkAccessManager *network;
    QMap<QNetworkReply*, Lyric*> lyrics;
};

#endif // AZLYRICS_H
