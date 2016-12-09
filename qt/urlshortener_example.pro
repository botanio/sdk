TEMPLATE = subdirs
SUBDIRS +=  botanioapi \
			example

#pro files
example.file = example/urlshortener/qtbotaniourlshortener.pro
botanioapi.file = qbotanio/qbotanio.pro

#dependencies
example.depends = botanioapi

QMAKE_CLEAN += Makefile