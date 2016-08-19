# update path...
message("biluna")

win32:QMAKE_CXXFLAGS += /FIbiluna/custom_defs.h
unix:QMAKE_CXXFLAGS += -include biluna/custom_defs.h

win32:RC_FILE = biluna/qcad_biluna.rc
macx:RC_FILE = biluna/qcad_biluna.icns

CONFIG += rs_custom
