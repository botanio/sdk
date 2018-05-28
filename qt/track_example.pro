TEMPLATE = subdirs
SUBDIRS +=  botanioapi \
			example

#pro files
example.file = example/track/qtbotaniotrack.pro
botanioapi.file = qbotanio/qbotanio.pro

#dependencies
example.depends = botanioapi

QMAKE_CLEAN += Makefile