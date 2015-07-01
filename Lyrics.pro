TARGET = harbour-lyrics

CONFIG += sailfishapp

SOURCES += \
    src/main.cpp \
    src/lyricsmanager.cpp \
    src/lyric.cpp \
    src/lyricswikiapi.cpp
#    src/chartlyricsapi.cpp

HEADERS += \
    src/lyricsmanager.h \
    src/lyric_p.h \
    src/lyric.h \
    src/lyricswikiapi.h
#    src/chartlyricsapi.h

OTHER_FILES += \
    qml/cover/CoverPage.qml \
    qml/Lyrics.qml \
    qml/pages/MainPage.qml \
    harbour-lyrics.desktop \
    rpm/harbour-lyrics.changes \
    rpm/harbour-lyrics.spec \
    rpm/harbour-lyrics.yaml \
    translations/*.ts

CONFIG += sailfishapp_i18n
TRANSLATIONS += translations/harbour-lyrics-it.ts

# LyricsWiki
QT += webkitwidgets

# ChartLyrics
#QT += xml
