# updated by Rutger for rs_prof rs_propertybrowser msvc2008

QT += core gui help network printsupport svg uitools widgets xml

UI_DIR      = .ui
OBJECTS_DIR = .obj
MOC_DIR     = .moc
RCC_DIR     = .res

CONFIG+=rs_qcad

# for QCad:
rs_qcad {
  #CONFIG += rs_scripting rs_no_flt rs_no_nif rs_no_dae rs_no_svg
  CONFIG += rs_no_flt rs_no_nif rs_no_dae rs_prof
  #CONFIG += rs_no_flt rs_no_nif rs_no_dae
  #CONFIG += rs_algo
  
  # for OOPL:
  #CONFIG += rs_oopl
  
  # for MTC:
  #CONFIG += rs_mtc

  # for Biluna
  #CONFIG += rs_biluna
}

# for CAM Expert:
rs_cam {
  CONFIG += rs_scripting rs_no_flt rs_no_nif rs_no_dae rs_no_svg
}

# for RWBE:
rs_rwbe {
  CONFIG += rs_arch rs_algo rs_no_svg
}

CONFIG -= debug_and_release
CONFIG += release
CONFIG -= debug
CONFIG -= thread

DEFINES += RS_NO_COMPLEX_ENTITIES

win32-msvc2008 {
  DEFINES += _CRT_SECURE_NO_DEPRECATE
}
    
win32-msvc2005 {
  rs_dynamic {
    DEFINES += RS_DYNAMIC
  }
  DEFINES -= UNICODE
  DEFINES += NDEBUG
  DEFINES -= NI_MEMORY_DEBUGGER
  DEFINES -= NI_USE_MEMORY_MANAGEMENT
}

unix {
  CONFIG += rs_no_flt
  CONFIG += rs_no_nif
  QMAKE_LFLAGS += -static-libgcc -L.
}

!rs_no_flt {
  message("Filter: OpenFlight FLT")
}
!rs_no_dae {
  message("Filter: Collada DAE")
}
!rs_no_nif {
  exists("$$(EGB_PATH)\SDK") {
    message("Filter: Gambryo NIF")
  }
  else {
    message("Gambryo SDK not found")
    CONFIG += rs_no_nif
  }
}
!rs_no_svg {
  exists("../qcadsvg") {
    message("Filter: SVG")
  }
  else {
    message("qcadsvg not found")
    CONFIG += rs_no_svg
  }
}

macx {
  exists(/Developer/SDKs/MacOSX10.5.sdk) {
    QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.5.sdk
    CONFIG+=x86 ppc
  }
  else {
    exists(/Developer/SDKs/MacOSX10.4u.sdk) {
      QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.4u.sdk
      CONFIG+=x86 ppc
    }
  }
  QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.3
}


#QTPLUGIN += qjpeg
#QTPLUGIN += qgif

win32 {
  #QTPLUGIN += qjpcodecs
  #QTPLUGIN += qcncodecs
  #QTPLUGIN += qkrcodecs
  #QTPLUGIN += qtwcodecs
}

QTDIRFIXED=$$(QTDIR)

win32-g++ {
  QTDIRFIXED=$$replace(QTDIRFIXED, /cygdrive/c, c:)
}

#message($$QTDIRFIXED)

# commented out by rutger
#LIBS += -L$$QTDIRFIXED/plugins/codecs -lqjpcodecs -lqcncodecs -lqkrcodecs -lqtwcodecs
#LIBS += -L$$QTDIRFIXED/plugins/imageformats -lqjpeg -lqgif

rs_cam {
  QT += qt3support
}

win32 {
  QMAKE_CFLAGS_THREAD -= -mthreads
  QMAKE_LFLAGS_THREAD -= -mthreads
  QMAKE_LFLAGS -= -mthreads
  #CONFIG += embed_manifest_exe
}


# detect prof version
rs_prof {
    message("Property Editor")
    DEFINES += RS_PROF
    
    win32 {
        win32-msvc2008 {
            #exists($$(QTDIR)\..\qtpropertybrowser-2.4-commercial\src\qtpropertybrowser.pri) {
            exists(../../workspace/thrd/qt-solutions/qtpropertybrowser/qtpropertybrowser.pri) {
                DEFINES += RS_PROPERTYEDITOR
                #INCLUDEPATH += $$(QTDIR)\..\qtpropertybrowser-2.4-commercial\src
                INCLUDEPATH += ../../workspace/thrd/qt-solutions/qtpropertybrowser
            }
            else {
                message("NO Property Editor")
            }
        }
        else {
            #exists(C:\qtpropertybrowser-2.4-commercial\src\qtpropertybrowser.pri) {
            exists(../../workspace/thrd/qt-solutions/qtpropertybrowser/qtpropertybrowser.pri) {
                DEFINES += RS_PROPERTYEDITOR
                #INCLUDEPATH += C:\qtpropertybrowser-2.4-commercial\src
                INCLUDEPATH += ../../workspace/thrd/qt-solutions/qtpropertybrowser
            }
            else {
                message("NO Property Editor")
            }
        }
    }
    else {
        #exists($$(QTDIR)/../qtpropertybrowser-2.4-commercial/src/qtpropertybrowser.pri) {
        exists(../../workspace/thrd/qt-solutions/qtpropertybrowser/qtpropertybrowser.pri) {
            DEFINES += RS_PROPERTYEDITOR
            #INCLUDEPATH += $$(QTDIR)/../qtpropertybrowser-2.4-commercial/src
            INCLUDEPATH += ../../workspace/thrd/qt-solutions/qtpropertybrowser
        }
        else {
            message("NO Property Editor")
        }
    }
}

# detect architecture module
rs_arch {
  exists(../rwbearch) {
	DEFINES += RS_ARCH
    INCLUDEPATH += ../../rwbearch/include
  }
  else {
    message("Architecture module not found")
    CONFIG -= rs_arch
  }
}

rs_algo {
  exists(../qcadalgo) {
	DEFINES += RS_ALGO
    INCLUDEPATH += \
		../qcadalgo/include/
  }
  else {
    message("Algorithm module qcadalgo not found")
	DEFINES -= RS_ALGO
  }
}

rs_no_flt {
  DEFINES += RS_NO_FLT
}

rs_no_nif {
  DEFINES += RS_NO_NIF
}

rs_no_dae {
  DEFINES += RS_NO_DAE
}

rs_no_svg {
  DEFINES += RS_NO_SVG
}

rs_demo {
  DEFINES += RS_DEMO
}

# detect fparser
!exists(../fparser) {
    DEFINES += RS_NO_FPARSER
}

# detect cam support
rs_cam {
  exists(../qcadcam) {
    DEFINES += RS_CAM
    DEFINES += RS_COMPAT
    INCLUDEPATH += ../qcadcam/src ../qcadcam/.ui
  }
  else {
    message("CAM module not found")
  }
}

# detect scripting support:
rs_scripting {
  exists(../qcadscripting) {
    DEFINES += RS_SCRIPTING
    INCLUDEPATH += ../qcadscripting/include
  }
  else {
    message("Scripting module not found")
    CONFIG -= rs_scripting
  }
}
    
INCLUDEPATH += ../mkspecs

#rs_biluna {
#  exists( biluna/custom_defs.pri ) {
#    include( biluna/custom_defs.pri )
#  }
#  else {
#    message("rs_biluna: biluna/custom_defs.pri not found")
#    CONFIG -= rs_biluna
#  }
#}

#rs_oopl {
#  exists( oopl/custom_defs.pri ) {
#    include( oopl/custom_defs.pri )
#  }
#  else {
#    message("rs_oopl: oopl/custom_defs.pri not found")
#    CONFIG -= rs_oopl
#  }
#}

#rs_mtc {
#  exists( mtc/custom_defs.pri ) {
#    include( mtc/custom_defs.pri )
#  }
#  else {
#    message("rs_mtc: mtc/custom_defs.pri not found")
#    CONFIG -= rs_mtc
#  }
#}

win32 {
  message("Platform: Windows")
  win32-msvc2008 {
    message("  Compiler: MSVC")
  }
  win32-g++ {
    message("  Compiler: g++")
  }
}

unix {
  message("Platform: Unix / Linux / OS X")
  macx {
    message("  Platform: Mac OS X")
    x86 {
      message("    Sub-Platform: x86")
    }
    ppc {
      message("    Sub-Platform: ppc")
    }
  }
}

rs_qcad {
  message("Product: QCad")
}

rs_cam {
  message("Product: CAM Expert")
}

rs_rwbe {
  message("Product: RWBE")
}

rs_scripting {
  message("Module: Scripting")
}

rs_biluna {
  message("Configuration: Biluna")
}

rs_oopl {
  message("Configuration: OOPL")
}

rs_mtc {
  message("Configuration: MTC")
}

rs_arch {
  message("Module: Architecture")
}

rs_algo {
  message("Module: Algorithms (triangulation, polygon clipping)")
}

rs_dynamic {
  message("Dynamic")
}

rs_demo {
  message("Demo")
}
