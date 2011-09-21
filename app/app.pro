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
    qt/proxystyle.h \
    src/documentsettings.h \
    src/version.h \
    src/wqcleardialog.h \
    src/wqnotification.h \
    src/macsearchwidget.h \
    src/macsearchwidgetcontainer.h
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
    src/macsearchwidgetcontainer.cpp
macx {
    QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.6.sdk
    TARGET = WordQuiz
    ICON = ../dist/mac/WordQuiz.icns
    DOC_ICON.files = ../dist/mac/WordQuizDoc.icns
    DOC_ICON.path = Contents/Resources
    QMAKE_BUNDLE_DATA += DOC_ICON
    SND_RES.files = src/sounds/correct.wav src/sounds/error.wav src/sounds/syntax_error.wav
    SND_RES.path = Contents/MacOS
    QMAKE_BUNDLE_DATA += SND_RES
    QMAKE_INFO_PLIST = ../dist/mac/info.plist
    LIBS += -framework Cocoa -framework IOKit -framework CoreFoundation -framework Foundation -framework Carbon
    OBJECTS_DIR += build/o/mac
    CONFIG += app_bundle
    OBJECTIVE_SOURCES += src/macsearchwidget.mm
}
unix:!macx {
    TARGET = wordquiz
    OBJECTS_DIR += build/o/unix
    target.path += /usr/bin/
    INSTALLS += target
    QT += dbus
}
win32 { 
    TARGET = WordQuiz
    OBJECTS_DIR += build/o/win32
    RC_FILE = ../dist/win/WordQuiz.rc
    CONFIG -= debug_and_release
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
INCLUDEPATH = src/ \
    kwordquiz/src/ \
    kdelibs/ \
    qt/ \
    ../lib/libkdeedu/keduvocdocument/
LIBS += ../lib/lib/libkeduvocdocument.a
DEPENDPATH = ../lib/lib/
RESOURCES += wordquiz.qrc
