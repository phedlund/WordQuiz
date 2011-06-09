FORMS = ui/mainwindow.ui \
    ui/dlgconfig.ui \
    ui/dlgcharacter.ui \
    kwordquiz/flashviewbase.ui \
    kwordquiz/multipleviewbase.ui \
    kwordquiz/qaviewbase.ui \
    kwordquiz/kwqscorewidgetbase.ui \
    ui/dlglanguage.ui \
    ui/wqcleardialog.ui
HEADERS = src/wordquiz.h \
    src/wqapplication.h \
    src/prefs.h \
    src/wordquizprefs.h \
    kwordquiz/kwqtablemodel.h \
    kwordquiz/kwqsortfiltermodel.h \
    kwordquiz/kwqtableview.h \
    kwordquiz/kwqtabledelegate.h \
    kwordquiz/kwqcommands.h \
    kwordquiz/flashview.h \
    kwordquiz/multipleview.h \
    kwordquiz/qaview.h \
    kwordquiz/wqprintdialogpage.h \
    kwordquiz/kwqquizmodel.h \
    kwordquiz/kwqscorewidget.h \
    kwordquiz/kwqcardscene.h \
    kwordquiz/kwqcardview.h \
    kwordquiz/kwqquizview.h \
    kwordquiz/version.h \
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
    src/qfmacsearchbox.h \
    src/wqcleardialog.h
SOURCES = src/main.cpp \
    src/wordquiz.cpp \
    src/wqapplication.cpp \
    src/prefs.cpp \
    src/wordquizprefs.cpp \
    kwordquiz/kwqtablemodel.cpp \
    kwordquiz/kwqsortfiltermodel.cpp \
    kwordquiz/kwqtableview.cpp \
    kwordquiz/kwqtabledelegate.cpp \
    kwordquiz/kwqcommands.cpp \
    kwordquiz/flashview.cpp \
    kwordquiz/multipleview.cpp \
    kwordquiz/qaview.cpp \
    kwordquiz/wqprintdialogpage.cpp \
    kwordquiz/kwqquizmodel.cpp \
    kwordquiz/kwqscorewidget.cpp \
    kwordquiz/kwqcardscene.cpp \
    kwordquiz/kwqcardview.cpp \
    kwordquiz/kwqquizview.cpp \
    kdelibs/krandomsequence.cpp \
    kdelibs/krandom.cpp \
    kdelibs/kcolorbutton.cpp \
    kdelibs/kfontrequester.cpp \
    qt/characterwidget.cpp \
    qt/lineedit.cpp \
    qt/searchlineedit.cpp \
    src/documentsettings.cpp \
    src/qfmacsearchbox.cpp \
    src/wqcleardialog.cpp
macx { 
    TARGET = WordQuiz
    ICON = ../dist/mac/WordQuiz.icns
    DOC_ICON.files = ../dist/mac/WordQuizDoc.icns
    DOC_ICON.path = Contents/Resources
    QMAKE_BUNDLE_DATA += DOC_ICON
    SND_RES.files = bin/alert.wav bin/cancel.wav bin/scrbar.wav
    SND_RES.path = Contents/MacOS
    QMAKE_BUNDLE_DATA += SND_RES
    QMAKE_INFO_PLIST = ../dist/mac/info.plist
    LIBS += -framework Carbon
    OBJECTS_DIR += build/o/mac
    CONFIG += app_bundle
}
unix { 
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
    kwordquiz/ \
    kdelibs/ \
    qt/ \
    ../keduvocdocument/src/
LIBS += ../keduvocdocument/lib/libkeduvocdocument.a
DEPENDPATH = ../keduvocdocument/lib/
RESOURCES += wordquiz.qrc
