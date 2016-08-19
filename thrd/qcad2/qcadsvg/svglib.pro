# ----------------------------------------------------------
# svglib.pro 2006-06-08 rutger
#            2007-09-12 andrew
# - for building svg library
# ----------------------------------------------------------

OBJECTS_DIR = .obj
MOC_DIR     = .moc

TEMPLATE	= lib
LANGUAGE	= C++
TARGET 		= qcadsvg
DESTDIR     = lib

CONFIG		+= qt staticlib warn_on debug
QT          += xml


INCLUDEPATH += \
               ./src \
               ../fparser/src \
               ../dxflib/src \
               ../qcadlib/src \
               ../qcadlib/src/actions \
               ../qcadlib/src/creation \
               ../qcadlib/src/debug \
               ../qcadlib/src/entities \
               ../qcadlib/src/fileio \
               ../qcadlib/src/filters \
               ../qcadlib/src/global \
               ../qcadlib/src/gui \
               ../qcadlib/src/information \
               ../qcadlib/src/math \
               ../qcadlib/src/modification \
               ../qcadlib/src/scripting \
               ../qcadlib/src/tools

HEADERS	= \
	./src/rb_cadattributes.h \
	./src/rb_filtersvg.h \
	./src/rb_svgcodes.h \
	./src/rb_xmlcodes.h \
	./src/rb_xmlreader.h \
	./src/rb_xmlsvg.h \
	./src/rb_xmlwriter.h 
	
SOURCES	= \
	./src/rb_cadattributes.cpp \
	./src/rb_filtersvg.cpp \
	./src/rb_svgcodes.cpp \
	./src/rb_xmlcodes.cpp \
	./src/rb_xmlsvg.cpp \
	./src/rb_xmlwriter.cpp 
