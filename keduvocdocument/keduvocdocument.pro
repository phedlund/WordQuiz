TEMPLATE = lib
HEADERS += src/keduvocarticle.h \
    src/keduvoccommon_p.h \
    src/keduvocconjugation.h \
    src/keduvoccontainer.h \
    src/keduvoccsvreader.h \
    src/keduvoccsvwriter.h \
    src/keduvocdeclension.h \
    src/keduvocdocument.h \
    src/keduvocexpression.h \
    src/keduvocidentifier.h \
    src/keduvockvtml2reader.h \
    src/keduvockvtml2writer.h \
    src/keduvockvtmlcompability.h \
    src/keduvockvtmlreader.h \
    src/keduvoclesson.h \
    src/keduvocmultiplechoice.h \
    src/keduvocpaukerreader.h \
    src/keduvocpersonalpronoun.h \
    src/keduvoctext.h \
    src/keduvoctranslation.h \
    src/keduvocvokabelnreader.h \
    src/keduvocwordtype.h \
    src/keduvocwordflags.h \
    src/keduvocwqlreader.h \
    src/keduvocxdxfreader.h \
    src/kvtml2defs.h \
    src/kvtmldefs.h \
    src/libkeduvocdocument_export.h \
    src/keduvocleitnerbox.h \
    klocale.h
SOURCES += src/keduvocconjugation.cpp \
    src/keduvoccontainer.cpp \
    src/keduvoccsvreader.cpp \
    src/keduvoccsvwriter.cpp \
    src/keduvocdeclension.cpp \
    src/keduvocdocument.cpp \
    src/keduvocexpression.cpp \
    src/keduvocidentifier.cpp \
    src/keduvockvtml2reader.cpp \
    src/keduvockvtml2writer.cpp \
    src/keduvockvtmlcompability.cpp \
    src/keduvockvtmlreader.cpp \
    src/keduvoclesson.cpp \
    src/keduvocmultiplechoice.cpp \
    src/keduvocpaukerreader.cpp \
    src/keduvocpersonalpronoun.cpp \
    src/keduvoctext.cpp \
    src/keduvoctranslation.cpp \
    src/keduvocvokabelnreader.cpp \
    src/keduvocwordtype.cpp \
    src/keduvocwqlreader.cpp \
    src/keduvocxdxfreader.cpp \
    src/keduvocarticle.cpp \
    src/keduvocleitnerbox.cpp
QT += xml \
    core
macx:LIBS += -framework Carbon
CONFIG += staticlib
DESTDIR = lib/
OBJECTS_DIR = release/
MOC_DIR = release/
TRANSLATIONS = ts/keduvocdocument_fr.ts \
    ts/keduvocdocument_se.ts
