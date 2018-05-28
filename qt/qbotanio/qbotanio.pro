#-------------------------------------------------
#
# Project created by QtCreator 2016-12-09T13:57:33
#
#-------------------------------------------------

QT       += network
QT       -= gui
TARGET = qbotanio
TEMPLATE = lib

DEFINES += QBOTANIO_LIBRARY
PRECOMPILED_HEADER = stdafx.h


QMAKE_CLEAN += Makefile
QMAKE_CLEAN += Makefile.$${TARGET}
QMAKE_CLEAN += Makefile.$${TARGET}.Release
QMAKE_CLEAN += Makefile.$${TARGET}.Debug
QMAKE_CLEAN += Makefile.Release
QMAKE_CLEAN += Makefile.Debug

CONFIG(release, debug|release) {
     DESTDIR            = ./Release
     MOC_DIR     	+= $${DESTDIR}/.moc
     OBJECTS_DIR 	+= $${DESTDIR}/.obj
     UI_DIR      	+= $${DESTDIR}/.ui
     RCC_DIR     	+= $${DESTDIR}/.rcc
}
else{
     DESTDIR            = ./Debug
     MOC_DIR     	+= $${DESTDIR}/.moc
     OBJECTS_DIR 	+= $${DESTDIR}/.obj
     UI_DIR      	+= $${DESTDIR}/.ui
     RCC_DIR     	+= $${DESTDIR}/.rcc
     TARGET             = $$join(TARGET,,,d)
     DEFINES            += _DEBUG
}

win32{
        QMAKE_CLEAN += $$DESTDIR/$${TARGET}.lib
        QMAKE_CLEAN += $$DESTDIR/$${TARGET}.dll
        QMAKE_CLEAN += $$DESTDIR/$${TARGET}.pdb
}
unix{
        QMAKE_CLEAN     += $$DESTDIR/lib$${TARGET}.so*
        QMAKE_CXXFLAGS  += -std=c++11

        target.path = /usr/lib
        INSTALLS += target
}

include(qbotanio.pri)
