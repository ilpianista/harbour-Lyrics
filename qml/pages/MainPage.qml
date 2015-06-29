/*
  The MIT License (MIT)

  Copyright (c) 2014-2015 Andrea Scarpino <me@andreascarpino.it>

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

import QtQuick 2.0
import Sailfish.Silica 1.0

Page {

    Connections {
        target: manager

        onSearchResult: {
            console.log("Got a lyric response, found: " + found);
            artist.enabled = true;
            song.enabled = true;
            busy.running = false;
            busy.visible = false;
            search.enabled = true;

            if (found) {
                songText.text = lyric.text;
            } else {
                songText.text = "Not found :-("
            }
        }
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        PullDownMenu {

            MenuItem {
                text: qsTr("Clear")

                onClicked: {
                    artist.text = "";
                    song.text = "";
                    songText.text = "";
                }
            }
        }

        Column {
            id: column
            x: Theme.horizontalPageMargin
            width: parent.width - Theme.horizontalPageMargin * 2

            PageHeader {
                title: qsTr("Lyrics")
            }

            TextField {
                id: artist
                width: parent.width
                focus: true
                placeholderText: qsTr("Artist")
            }

            TextField {
                id: song
                width: parent.width
                placeholderText: qsTr("Song")
            }

            BusyIndicator {
                id: busy
                visible: false
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Button {
                id: search
                text: qsTr("Search")
                anchors.horizontalCenter: parent.horizontalCenter

                onClicked: {
                    if (artist.text.length !== 0 && song.text.length !== 0) {
                        songText.text = "";
                        artist.enabled = false;
                        song.enabled = false;
                        busy.visible = true;
                        busy.running = true;
                        search.enabled = false;
                        manager.search(artist.text, song.text);
                    }
                }
            }

            Text {
                id: songText
                width: parent.width
                color: Theme.secondaryColor
                wrapMode: Text.Wrap
            }
        }

        VerticalScrollDecorator {}
    }
}
