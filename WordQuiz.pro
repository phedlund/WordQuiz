TEMPLATE = subdirs
SUBDIRS = lib \
          app

CONFIG += ordered
#macx:LIBS += -framework Carbon
#TODO

#Follow http://bugreports.qt.nokia.com/browse/QTBUG-6562
