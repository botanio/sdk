QT += core network
QT -= gui
CONFIG += c++11
TARGET = qtbotaniotrack
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

SOURCES += main.cpp 

INCLUDEPATH += ./../../qbotanio

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
     LIBS               += -L../../qbotanio/Release -lqbotanio
}
else{
     DESTDIR            = ./Debug
     MOC_DIR     	+= $${DESTDIR}/.moc
     OBJECTS_DIR 	+= $${DESTDIR}/.obj
     UI_DIR      	+= $${DESTDIR}/.ui
     RCC_DIR     	+= $${DESTDIR}/.rcc
     TARGET             = $$join(TARGET,,,d)
     LIBS               += -L../../qbotanio/Debug -lqbotaniod
}
unix{
    QMAKE_CXXFLAGS  +=-std=c++11
    target.path = /usr/bin
    INSTALLS += target
}