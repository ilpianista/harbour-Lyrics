TARGET = harbour-lyrics

CONFIG += sailfishapp

SOURCES += \
    src/main.cpp \
    src/lyricsmanager.cpp \
    src/lyric.cpp \
    src/chartlyricsapi.cpp \
    src/geniusapi.cpp \
    src/lyricswikiapi.cpp \
    src/provider.cpp \
    src/lyricsmaniaapi.cpp

HEADERS += \
    src/lyricsmanager.h \
    src/lyric_p.h \
    src/lyric.h \
    src/chartlyricsapi.h \
    src/geniusapi.h \
    src/geniusapi_secret.h \
    src/lyricswikiapi.h \
    src/provider.h \
    src/lyricsmaniaapi.h

OTHER_FILES += \
    qml/cover/CoverPage.qml \
    qml/Lyrics.qml \
    qml/pages/MainPage.qml \
    qml/pages/Settings.qml \
    harbour-lyrics.desktop \
    rpm/harbour-lyrics.changes \
    rpm/harbour-lyrics.spec \
    rpm/harbour-lyrics.yaml \
    translations/*.ts

CONFIG += sailfishapp_i18n
TRANSLATIONS += translations/harbour-lyrics-it.ts \
    translations/harbour-lyrics-ca.ts \
    translations/harbour-lyrics-cs.ts \
    translations/harbour-lyrics-fr_FR.ts

# LyricsWiki, Genius, LyricsMania
QT += webkitwidgets

# ChartLyrics
QT += xml
