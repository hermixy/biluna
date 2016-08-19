QT += xml
TEMPLATE = app
CONFIG += qt warn_on link_prl rs_prof
DEFINE += RS_PROF RS_PROPERTYEDITOR


# global defs
exists( ../mkspecs/defs.pro ):include( ../mkspecs/defs.pro )

# reseller specifics:
rs_biluna {
  exists( ../mkspecs/biluna/qcad_custom.qrc ) {
    RESOURCES += ../mkspecs/biluna/qcad_custom.qrc
  }
  else {
    message("rs_oopl: '../mkspecs/biluna/qcad_custom.qrc' not found")
  }
}

rs_mtc {
  exists( ../mkspecs/mtc/qcad_custom.qrc ) {
    RESOURCES += ../mkspecs/mtc/qcad_custom.qrc
  }
  else {
    message("rs_mtc: '../mkspecs/mtc/qcad_custom.qrc' not found")
  }
}

rs_oopl {
  exists( ../mkspecs/oopl/qcad_custom.qrc ) {
    RESOURCES += ../mkspecs/oopl/qcad_custom.qrc
  }
  else {
    message("rs_oopl: '../mkspecs/oopl/qcad_custom.qrc' not found")
  }
}

# Application specific targets:

# CAM Expert:
rs_cam {
  rs_demo {
    macx {
      TARGET = CAMExpertDemo
    }
    else {
      TARGET = camexpert_demo
    }
  }
  else {
    macx {
      TARGET = CAMExpert
    }
    else {
      TARGET = camexpert
    }
  }
}

# QCad:
else {
  rs_demo {
    macx {
      TARGET = QCadDemo
    }
    else {
      TARGET = qcad_demo
    }
  }
  else {
    macx {
      TARGET = Cad
    }
    else {
      TARGET = cad
    }
  }
}


# Application specific defines:

# CAM Expert:
rs_cam {
	rs_demo {
    	DEFINES += QC_APPKEY="\\\"/CAMExpertDemo\\\"" QC_APPDIR="\\\"camexpertdemo\\\""
		DEFINES += QC_APPNAME="\\\"CAM Expert Demo\\\""
	}
	else {
    	DEFINES += QC_APPKEY="\\\"/CAMExpert\\\"" QC_APPDIR="\\\"camexpert\\\""
		DEFINES += QC_APPNAME="\\\"CAM\ Expert\\\""
	}
}

# QCad:
else {
	rs_custom {
		#allows override app name with custom one
	}
	else {
		rs_demo {
                        DEFINES += QC_APPKEY="\\\"/CadDemo\\\"" QC_APPDIR="\\\"caddemo\\\""
			win32 {
                            DEFINES += QC_APPNAME="\\\"Cad\ Demo\\\""
			}
			else {
                            DEFINES += QC_APPNAME="\\\"Cad\ Demo\\\""
			}
		}
		else {
                        DEFINES += QC_APPKEY="\\\"/Cad\\\"" QC_APPDIR="\\\"cad\\\""
                        DEFINES += QC_APPNAME="\\\"Cad\\\""
		}
	}
}


# libraries:
#LIBS += -lQtAssistantClient

rs_scripting {
    LIBS += -L../qcadscripting/lib -lqcadscripting
}
LIBS += \
        -L../qcadguiqt/lib -lqcadguiqt \
        -L../qcadactions/lib -lqcadactions
#rs_prof {
    #LIBS +=  -L../qcadprof/lib -lqcadprof
    LIBS +=  -L../qcadprop/lib -lqcadprop
#}
rs_arch {
    LIBS +=  -L../rwbearch/lib -lrwbearch
}
LIBS += -L../qcadlib/lib -lqcad
rs_algo {
    LIBS +=  -L../qcadalgo/lib -lqcadalgo
}
LIBS += \
        -L../dxflib/lib -llibdxf \
        -L../fparser/lib -lfparser
rs_cam {
    LIBS += -L../qcadcam/lib -lqcadcam
}
!rs_no_svg {
    LIBS += -L../qcadsvg/lib -lqcadsvg
}

macx {
    QMAKE_INFO_PLIST = src/Info.plist
}

# icon
!rs_custom {
        win32:RC_FILE = src/app_ico.rc
	macx:RC_FILE = src/qcad.icns
}

# include paths:
INCLUDEPATH += \
    ./src/modules \
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
    ../qcadlib/src/tools \
    ../qcadsvg/src \
    ../qcadactions/src \
    ../qcadactions/src/blocks \
    ../qcadactions/src/debugging \
    ../qcadactions/src/dimension \
    ../qcadactions/src/draw \
    ../qcadactions/src/edit \
    ../qcadactions/src/file \
    ../qcadactions/src/info \
    ../qcadactions/src/layers \
    ../qcadactions/src/misc \
    ../qcadactions/src/modify \
    ../qcadactions/src/options \
    ../qcadactions/src/script \
    ../qcadactions/src/select \
    ../qcadactions/src/snap \
    ../qcadactions/src/view \
    ../qcadactions/src/zoom \
    ../qcadguiqt/src \
    ../qcadguiqt/src/dialogs \
    ../qcadguiqt/src/toolbars \
    ../qcadguiqt/src/tooloptions \
    ../qcadguiqt/src/widgets \
    ../qcadguiqt/.ui \
    ../qcadactions/src \
    ../fparser/src \
    ../qcadguiqt/src \
    ../qcadscripting/src \
    ../qcadscripting/src/qsa


HEADERS = ./src/qc_applicationwindow.h \
          ./src/qc_application.h \
          ./src/main.h
SOURCES = ./src/qc_applicationwindow.cpp \
          ./src/qc_application.cpp \
          ./src/main.cpp 
TRANSLATIONS = \
    ./src/ts/qcad_bg.ts \
    ./src/ts/qcad_cs.ts \
    ./src/ts/qcad_cy.ts \
    ./src/ts/qcad_da.ts \
    ./src/ts/qcad_de.ts \
    ./src/ts/qcad_el.ts \
    ./src/ts/qcad_en.ts \
    ./src/ts/qcad_es.ts \
    ./src/ts/qcad_et.ts \
    ./src/ts/qcad_fr.ts \
    ./src/ts/qcad_hu.ts \
    ./src/ts/qcad_id.ts \
    ./src/ts/qcad_it.ts \
    ./src/ts/qcad_lt.ts \
    ./src/ts/qcad_ja.ts \
    ./src/ts/qcad_nl.ts \
    ./src/ts/qcad_no.ts \
    ./src/ts/qcad_pa.ts \
    ./src/ts/qcad_pl.ts \
    ./src/ts/qcad_pt.ts \
    ./src/ts/qcad_ru.ts \
    ./src/ts/qcad_sk.ts \
    ./src/ts/qcad_sv.ts \
    ./src/ts/qcad_tr.ts \
    ./src/ts/qcad_zh.ts

RESOURCES += src/qcad.qrc

DESTDIR     = .

UI_DIR      = ui
OBJECTS_DIR = .obj
MOC_DIR     = .moc
RCC_DIR     = .res

# load scripting support:
rs_scripting {
  win32 {
    load( qsa )
  }
  unix {
    LIBS += -lqsa
  }
}
