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
    id: page

    allowedOrientations: Orientation.All

    Column {
        x: Theme.horizontalPageMargin
        width: parent.width - Theme.horizontalPageMargin * 2
        spacing: Theme.paddingMedium

        PageHeader {
            title: qsTr("Settings")
        }

        ComboBox {
            id: provider
            width: page.width
            label: qsTr("Provider")

            menu: ContextMenu {

                MenuItem {
                    text: "ChartLyrics"
                }

                MenuItem {
                    text: "Genius"
                }

                MenuItem {
                    text: "LyricsMania"
                }
            }
        }

        Button {
            text: qsTr("Clear cache")
            anchors.horizontalCenter: parent.horizontalCenter

            onClicked: manager.clearCache();
        }
    }

    Component.onCompleted: {
        var api = manager.getProvider();
        if (api === "ChartLyrics") {
            provider.currentIndex = 0;
        } else if (api === "Genius") {
            provider.currentIndex = 1;
        } else {
            provider.currentIndex = 2;
        }
    }

    onStatusChanged: {
        if (status === PageStatus.Deactivating) {
            switch (provider.currentIndex) {
            case 0: manager.setProvider("ChartLyrics"); break;
            case 1: manager.setProvider("Genius"); break;
            case 2:
            default: manager.setProvider("LyricsMania");
            }
        }
    }
}
