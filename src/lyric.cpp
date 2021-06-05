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

#include "lyric.h"

#include "lyric_p.h"

Lyric::Lyric(QObject *parent) : QObject(parent)
  , d(new LyricPrivate)
{
}

Lyric::~Lyric()
{
    delete d;
}

QString Lyric::artist() const
{
    return d->artist;
}

void Lyric::setArtist(const QString &artist)
{
    d->artist = artist;
}

QString Lyric::song() const
{
    return d->song;
}

void Lyric::setSong(const QString &song)
{
    d->song = song;
}

QString Lyric::text() const
{
    return d->text;
}

void Lyric::setText(const QString &text)
{
    d->text = text;
}
