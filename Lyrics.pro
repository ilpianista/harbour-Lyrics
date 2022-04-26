TARGET = harbour-lyrics

CONFIG += sailfishapp

SOURCES += \
    src/main.cpp \
    src/lyricsmanager.cpp \
    src/lyric.cpp \
    src/chartlyricsapi.cpp \
    src/provider.cpp

HEADERS += \
    src/lyricsmanager.h \
    src/lyric_p.h \
    src/lyric.h \
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
    translations/*.ts

CONFIG += sailfishapp_i18n

TRANSLATIONS += \
    translations/harbour-lyrics-ar.ts \
    translations/harbour-lyrics-ca.ts \
    translations/harbour-lyrics-cs.ts \
    translations/harbour-lyrics-de.ts \
    translations/harbour-lyrics-el.ts \
    translations/harbour-lyrics-es.ts \
    translations/harbour-lyrics-et.ts \
    translations/harbour-lyrics-fi.ts \
    translations/harbour-lyrics-fr_FR.ts \
    translations/harbour-lyrics-hr.ts \
    translations/harbour-lyrics-it.ts \
    translations/harbour-lyrics-nb_NO.ts \
    translations/harbour-lyrics-nl_BE.ts \
    translations/harbour-lyrics-nl.ts \
    translations/harbour-lyrics-ru.ts \
    translations/harbour-lyrics-sr.ts \
    translations/harbour-lyrics-sv.ts \
    translations/harbour-lyrics-tr.ts \
    translations/harbour-lyrics-zh_Hant.ts

# ChartLyrics
QT += xml
