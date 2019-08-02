include(../common.pri)

CONFIG += console stl

SOURCES += main.cpp \
			$$VCGDIR/wrap/ply/plylib.cpp
