TEMPLATE = lib
HEADERS += libkdeedu/keduvocdocument/keduvocarticle.h \
    libkdeedu/keduvocdocument/keduvoccommon_p.h \
    libkdeedu/keduvocdocument/keduvocconjugation.h \
    libkdeedu/keduvocdocument/keduvoccontainer.h \
    libkdeedu/keduvocdocument/keduvoccsvreader.h \
    libkdeedu/keduvocdocument/keduvoccsvwriter.h \
    libkdeedu/keduvocdocument/keduvocdeclension.h \
    libkdeedu/keduvocdocument/keduvocdocument.h \
    libkdeedu/keduvocdocument/keduvocexpression.h \
    libkdeedu/keduvocdocument/keduvocidentifier.h \
    libkdeedu/keduvocdocument/keduvockvtml2reader.h \
    libkdeedu/keduvocdocument/keduvockvtml2writer.h \
    libkdeedu/keduvocdocument/keduvockvtmlcompability.h \
    libkdeedu/keduvocdocument/keduvockvtmlreader.h \
    libkdeedu/keduvocdocument/keduvoclesson.h \
    libkdeedu/keduvocdocument/keduvocmultiplechoice.h \
    libkdeedu/keduvocdocument/keduvocpaukerreader.h \
    libkdeedu/keduvocdocument/keduvocpersonalpronoun.h \
    libkdeedu/keduvocdocument/keduvoctext.h \
    libkdeedu/keduvocdocument/keduvoctranslation.h \
    libkdeedu/keduvocdocument/keduvocvokabelnreader.h \
    libkdeedu/keduvocdocument/keduvocwordtype.h \
    libkdeedu/keduvocdocument/keduvocwordflags.h \
    libkdeedu/keduvocdocument/keduvocwqlreader.h \
    libkdeedu/keduvocdocument/keduvocxdxfreader.h \
    libkdeedu/keduvocdocument/kvtml2defs.h \
    libkdeedu/keduvocdocument/kvtmldefs.h \
    libkdeedu/keduvocdocument/libkeduvocdocument_export.h \
    libkdeedu/keduvocdocument/keduvocleitnerbox.h
SOURCES += libkdeedu/keduvocdocument/keduvocconjugation.cpp \
    libkdeedu/keduvocdocument/keduvoccontainer.cpp \
    libkdeedu/keduvocdocument/keduvoccsvreader.cpp \
    libkdeedu/keduvocdocument/keduvoccsvwriter.cpp \
    libkdeedu/keduvocdocument/keduvocdeclension.cpp \
    libkdeedu/keduvocdocument/keduvocdocument.cpp \
    libkdeedu/keduvocdocument/keduvocexpression.cpp \
    libkdeedu/keduvocdocument/keduvocidentifier.cpp \
    libkdeedu/keduvocdocument/keduvockvtml2reader.cpp \
    libkdeedu/keduvocdocument/keduvockvtml2writer.cpp \
    libkdeedu/keduvocdocument/keduvockvtmlcompability.cpp \
    libkdeedu/keduvocdocument/keduvockvtmlreader.cpp \
    libkdeedu/keduvocdocument/keduvoclesson.cpp \
    libkdeedu/keduvocdocument/keduvocmultiplechoice.cpp \
    libkdeedu/keduvocdocument/keduvocpaukerreader.cpp \
    libkdeedu/keduvocdocument/keduvocpersonalpronoun.cpp \
    libkdeedu/keduvocdocument/keduvoctext.cpp \
    libkdeedu/keduvocdocument/keduvoctranslation.cpp \
    libkdeedu/keduvocdocument/keduvocvokabelnreader.cpp \
    libkdeedu/keduvocdocument/keduvocwordtype.cpp \
    libkdeedu/keduvocdocument/keduvocwqlreader.cpp \
    libkdeedu/keduvocdocument/keduvocxdxfreader.cpp \
    libkdeedu/keduvocdocument/keduvocarticle.cpp \
    libkdeedu/keduvocdocument/keduvocleitnerbox.cpp
QT += xml \
    core
CONFIG += staticlib
DESTDIR = lib/
OBJECTS_DIR = build/o/
MOC_DIR = build/moc/
TARGET = keduvocdocument
