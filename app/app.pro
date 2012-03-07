FORMS = ui/mainwindow.ui \
    ui/dlgconfig.ui \
    ui/dlgcharacter.ui \
    kwordquiz/src/flashviewbase.ui \
    kwordquiz/src/multipleviewbase.ui \
    kwordquiz/src/qaviewbase.ui \
    kwordquiz/src/kwqscorewidgetbase.ui \
    ui/dlglanguage.ui \
    ui/wqcleardialog.ui
HEADERS = src/wordquiz.h \
    src/wqapplication.h \
    src/prefs.h \
    src/wordquizprefs.h \
    kwordquiz/src/kwqtablemodel.h \
    kwordquiz/src/kwqsortfiltermodel.h \
    kwordquiz/src/kwqtableview.h \
    kwordquiz/src/kwqtabledelegate.h \
    kwordquiz/src/kwqcommands.h \
    kwordquiz/src/flashview.h \
    kwordquiz/src/multipleview.h \
    kwordquiz/src/qaview.h \
    kwordquiz/src/wqprintdialogpage.h \
    kwordquiz/src/kwqquizmodel.h \
    kwordquiz/src/kwqscorewidget.h \
    kwordquiz/src/kwqcardscene.h \
    kwordquiz/src/kwqcardview.h \
    kwordquiz/src/kwqquizview.h \
    kwordquiz/src/kwqpixmapitem.h \
    kwordquiz/src/version.h \
    kdelibs/krandomsequence.h \
    kdelibs/krandom.h \
    kdelibs/kcolorbutton.h \
    kdelibs/kfontrequester.h \
    qt/characterwidget.h \
    qt/lineedit_p.h \
    qt/lineedit.h \
    qt/searchlineedit.h \
    src/documentsettings.h \
    src/version.h \
    src/wqcleardialog.h \
    src/wqnotification.h \
    src/macsearchwidget.h \
    src/searchwidgetcontainer.h \
    src/SparkleAutoUpdater.h \
    src/CocoaInitializer.h \
    src/AutoUpdater.h \
    src/kwqkeyboard.h
SOURCES = src/main.cpp \
    src/wordquiz.cpp \
    src/wqapplication.cpp \
    src/prefs.cpp \
    src/wordquizprefs.cpp \
    kwordquiz/src/kwqtablemodel.cpp \
    kwordquiz/src/kwqsortfiltermodel.cpp \
    kwordquiz/src/kwqtableview.cpp \
    kwordquiz/src/kwqtabledelegate.cpp \
    kwordquiz/src/kwqcommands.cpp \
    kwordquiz/src/flashview.cpp \
    kwordquiz/src/multipleview.cpp \
    kwordquiz/src/qaview.cpp \
    kwordquiz/src/wqprintdialogpage.cpp \
    kwordquiz/src/kwqquizmodel.cpp \
    kwordquiz/src/kwqscorewidget.cpp \
    kwordquiz/src/kwqcardscene.cpp \
    kwordquiz/src/kwqcardview.cpp \
    kwordquiz/src/kwqquizview.cpp \
    kwordquiz/src/kwqpixmapitem.cpp \
    kdelibs/krandomsequence.cpp \
    kdelibs/krandom.cpp \
    kdelibs/kcolorbutton.cpp \
    kdelibs/kfontrequester.cpp \
    qt/characterwidget.cpp \
    qt/lineedit.cpp \
    qt/searchlineedit.cpp \
    src/documentsettings.cpp \
    src/wqcleardialog.cpp \
    src/wqnotification.cpp \
    src/searchwidgetcontainer.cpp \
    src/AutoUpdater.cpp
macx {
    QMAKE_MAC_SDK = /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.6.sdk
    TARGET = WordQuiz
    ICON = ../dist/mac/WordQuiz.icns
    DOC_ICON.files = ../dist/mac/WordQuizDoc.icns ../dist/mac/dsa_pub.pem
    DOC_ICON.path = Contents/Resources
    QMAKE_BUNDLE_DATA += DOC_ICON
    SND_RES.files = src/sounds/correct.wav src/sounds/error.wav src/sounds/syntax_error.wav
    SND_RES.path = Contents/MacOS
    QMAKE_BUNDLE_DATA += SND_RES
    SPARKLE.files = /Library/Frameworks/Sparkle.framework
    SPARKLE.path = Contents/Frameworks
    QMAKE_BUNDLE_DATA += SPARKLE
    QMAKE_INFO_PLIST = ../dist/mac/info.plist
    LIBS += -framework Cocoa -framework IOKit -framework CoreFoundation -framework Foundation -framework Carbon -framework Sparkle
    OBJECTS_DIR += build/o/mac
    CONFIG += app_bundle
    OBJECTIVE_SOURCES += src/macsearchwidget.mm \
    src/SparkleAutoUpdater.mm \
    src/CocoaInitializer.mm \
    src/kwqkeyboard_mac.mm
}
unix:!macx {
    SOURCES += src/kwqkeyboard_x11.cpp
    TARGET = wordquiz
    OBJECTS_DIR += build/o/unix
    target.path += /usr/bin/
    INSTALLS += target
    QT += dbus
    LIBS += -lX11
}
win32 { 
    SOURCES += kwqkeyboard_win.cpp
    TARGET = WordQuiz
    OBJECTS_DIR += build/o/win32
    RC_FILE = ../dist/win/WordQuiz.rc
    CONFIG -= debug_and_release
    INCLUDEPATH += $$PWD/../../WinSparkle-0.3
    LIBS += $$PWD/../../WinSparkle-0.3/WinSparkle.lib
}
TEMPLATE = app
CONFIG += warn_on \
    debug_and_release
QT += core \
    xml \
    gui \
    phonon \
    opengl
DESTDIR = bin
UI_DIR = build/ui
MOC_DIR = build/moc
RCC_DIR = build/rcc
INCLUDEPATH += src/ \
    kwordquiz/src/ \
    kdelibs/ \
    qt/ \
    ../lib/libkdeedu/keduvocdocument/
LIBS += ../lib/lib/libkeduvocdocument.a
DEPENDPATH = ../lib/lib/
RESOURCES += wordquiz.qrc

win32 {
    WQ_VERSION_YEAR=$$system(C:/Progra~1/GnuWin32/bin/date +"%Y")
    WQ_VERSION_MONTH=$$system(C:/Progra~1/GnuWin32/bin/date +"%m")
    WQ_VERSION_DAY=$$system(C:/Progra~1/GnuWin32/bin/date +"%d")
    WQ_PUB_DATE=$$system('C:/Progra~1/GnuWin32/bin/date -u "+%a, %d %b %Y %X %z"')
}
unix {
    WQ_VERSION_YEAR=$$system(date +"%Y")
    WQ_VERSION_MONTH=$$system(date +"%m")
    WQ_VERSION_DAY=$$system(date +"%d")
    WQ_PUB_DATE=$$system('date -u "+%a, %d %b %Y %X %z"')
}

vers.input = $$PWD/src/version.h.in
vers.output = $$PWD/src/version.h

QMAKE_SUBSTITUTES += vers

gpl.input = $$PWD/../dist/LICENSE.GPL2.in
gpl.output = $$PWD/../dist/LICENSE.GPL2

QMAKE_SUBSTITUTES += gpl

plist.input = $$PWD/../dist/mac/info.plist.in
plist.output = $$PWD/../dist/mac/info.plist

QMAKE_SUBSTITUTES += plist

sparkle.input = $$PWD/../dist/mac/appcast.xml.in
sparkle.output = $$PWD/../dist/mac/appcast.xml

QMAKE_SUBSTITUTES += sparkle

winsparkle.input = $$PWD/../dist/win/appcastwin.xml.in
winsparkle.output = $$PWD/../dist/win/appcastwin.xml

QMAKE_SUBSTITUTES += winsparkle

iss.input = $$PWD/../dist/win/iss_include.txt.in
iss.output = $$PWD/../dist/win/iss_include.txt

QMAKE_SUBSTITUTES += iss

