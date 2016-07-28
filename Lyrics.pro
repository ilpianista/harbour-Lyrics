TARGET = harbour-lyrics

CONFIG += sailfishapp

QT += dbus

DBUS_INTERFACES += org.mpris.MediaPlayer2.Player.xml

SOURCES += \
    src/main.cpp \
    src/lyricsmanager.cpp \
    src/lyric.cpp \
    src/mediaplayerscanner.cpp \
    src/chartlyricsapi.cpp \
    src/provider.cpp

HEADERS += \
    src/lyricsmanager.h \
    src/lyric_p.h \
    src/lyric.h \
    src/mediaplayerscanner.h \
    src/chartlyricsapi.h \
    src/provider.h

OTHER_FILES += \
    qml/cover/CoverPage.qml \
    qml/Lyrics.qml \
    qml/pages/MainPage.qml \
    qml/pages/Settings.qml \
    harbour-lyrics.desktop \
    rpm/harbour-lyrics.changes \
    rpm/harbour-lyrics.spec \
    rpm/harbour-lyrics.yaml \
    org.mpris.MediaPlayer2.Player.xml \
    translations/*.ts

CONFIG += sailfishapp_i18n

# ChartLyrics
QT += xml
