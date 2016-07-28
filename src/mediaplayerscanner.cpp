/*
  The MIT License (MIT)

  Copyright (c) 2016 Andrea Scarpino <me@andreascarpino.it>

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

#include "mediaplayerscanner.h"

#include <QTimer>

const QString MediaPlayerScanner::MP_DBUS_SERVICE(QLatin1String("org.mpris.MediaPlayer2.jolla-mediaplayer"));
const QString MediaPlayerScanner::MP_DBUS_DAEMON_PATH(QLatin1String("/org/mpris/MediaPlayer2"));

MediaPlayerScanner::MediaPlayerScanner(QObject *parent) :
    QObject(parent)
  , impface(MediaPlayerScanner::MP_DBUS_SERVICE, MediaPlayerScanner::MP_DBUS_DAEMON_PATH, QDBusConnection::sessionBus())
  , m_timer(new QTimer())
{
    fetchInfo();

    m_timer->start(5 * 1000);
    connect(m_timer, &QTimer::timeout, this, &MediaPlayerScanner::fetchInfo);
}

MediaPlayerScanner::~MediaPlayerScanner()
{
    delete m_timer;
}

void MediaPlayerScanner::fetchInfo()
{
    const QVariantMap map = impface.metadata();
    const QStringList artists(map["xesam:artist"].toStringList());
    const QString title(map["xesam:title"].toString());

    if (!artists.isEmpty() && !title.isEmpty()) {
        Q_EMIT mediaPlayerInfo(artists.at(0), title);
    }
}
