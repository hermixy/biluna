TEMPLATE    = lib
DESTDIR     = lib
CONFIG      += qt staticlib warn_on

exists( ../mkspecs/defs.pro ) {
    include( ../mkspecs/defs.pro )
}

INCLUDEPATH += \
    ./src \
    ./src/blocks \
    ./src/debugging \
    ./src/dimension \
    ./src/draw \
    ./src/edit \
    ./src/file \
    ./src/info \
    ./src/layers \
    ./src/misc \
    ./src/modify \
    ./src/select \
    ./src/snap \
    ./src/view \
    ./src/zoom \
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
    ../qcadguiqt/src \
    ../qcadguiqt/.ui \
    ../qcadguiqt/src/dialogs \
    ../qcadguiqt/src/toolbars \
    ../qcadguiqt/src/tooloptions \
    ../qcadguiqt/src/widgets \
    ../qcadprof/src \
    ../qcadprof/.ui \
    ../fparser/src \
    ../qcadguiqt/src

HEADERS = \
    ./src/blocks/rs_actionblocksadd.h \
    ./src/blocks/rs_actionblocksattributes.h \
    ./src/blocks/rs_actionblockscreate.h \
    ./src/blocks/rs_actionblocksedit.h \
    ./src/blocks/rs_actionblockseditinsert.h \
    ./src/blocks/rs_actionblocksinsert.h \
    ./src/blocks/rs_actionblocksremove.h \
    ./src/blocks/rs_actionblockstoggleview.h \
    ./src/blocks/rs_actionblocksshowall.h \
    ./src/blocks/rs_actionblockshideall.h \
    ./src/debugging/rs_actiondebugentities.h \
    ./src/debugging/rs_actiondebugguiactions.h \
    ./src/debugging/rs_actiondebugresize.h \
    ./src/dimension/rs_actiondimaligned.h \
    ./src/dimension/rs_actiondimangular.h \
    ./src/dimension/rs_actiondimdiametric.h \
    ./src/dimension/rs_actiondimension.h \
    ./src/dimension/rs_actiondimleader.h \
    ./src/dimension/rs_actiondimlinear.h \
    ./src/dimension/rs_actiondimradial.h \
    ./src/dimension/rs_actiondimordinate.h \
    ./src/dimension/rs_actiondimhorizontal.h \
    ./src/dimension/rs_actiondimvertical.h \
    ./src/dimension/rs_actiontoolregeneratedimensions.h \
    ./src/draw/rs_actiondrawarc3p.h \
    ./src/draw/rs_actiondrawarc.h \
    ./src/draw/rs_actiondrawarctangential.h \
    ./src/draw/rs_actiondrawcircle2p.h \
    ./src/draw/rs_actiondrawcircle3p.h \
    ./src/draw/rs_actiondrawcirclecr.h \
    ./src/draw/rs_actiondrawcircle.h \
    ./src/draw/rs_actiondrawellipsearcaxis.h \
    ./src/draw/rs_actiondrawellipseaxis.h \
    ./src/draw/rs_actiondrawhatch.h \
    ./src/draw/rs_actiondrawimage.h \
    ./src/draw/rs_actiondrawlineangle.h \
    ./src/draw/rs_actiondrawlinebisector.h \
    ./src/draw/rs_actiondrawlinefree.h \
    ./src/draw/rs_actiondrawline.h \
    ./src/draw/rs_actiondrawlinehorizontal.h \
    ./src/draw/rs_actiondrawlinehorvert.h \
    ./src/draw/rs_actiondrawlineparallel.h \
    ./src/draw/rs_actiondrawarcparallel.h \
    ./src/draw/rs_actiondrawcircleparallel.h \
    ./src/draw/rs_actiondrawlineparallelthrough.h \
    ./src/draw/rs_actiondrawlinepolygon2.h \
    ./src/draw/rs_actiondrawlinepolygon.h \
    ./src/draw/rs_actiondrawlinerectangle.h \
    ./src/draw/rs_actiondrawlinerelangle.h \
    ./src/draw/rs_actiondrawlinetangent1.h \
    ./src/draw/rs_actiondrawlinetangent2.h \
    ./src/draw/rs_actiondrawlinevertical.h \
    ./src/draw/rs_actiondrawlineorthogonal.h \
    ./src/draw/rs_actiondrawpoint.h \
    ./src/draw/rs_actiondrawspline.h \
    ./src/draw/rs_actiondrawtext.h \
    ./src/edit/rs_actioneditcopy.h \
    ./src/edit/rs_actioneditcopyautoref.h \
    ./src/edit/rs_actioneditcut.h \
    ./src/edit/rs_actioneditcutautoref.h \
    ./src/edit/rs_actioneditpaste.h \
    ./src/edit/rs_actioneditundo.h \
    ./src/edit/rs_actioneditredo.h \
    ./src/file/rs_actionfileclose.h \
    ./src/file/rs_actionfileexport.h \
    ./src/file/rs_actionfilenew.h \
    ./src/file/rs_actionfileopen.h \
    ./src/file/rs_actionfilepdf.h \
    ./src/file/rs_actionfilesvg.h \
    ./src/file/rs_actionfileprint.h \
    ./src/file/rs_actionfileprintpreview.h \
    ./src/file/rs_actionfilequit.h \
    ./src/file/rs_actionfilesaveas.h \
    ./src/file/rs_actionfilesave.h \
    ./src/info/rs_actioninfoangle.h \
    ./src/info/rs_actioninfoarea.h \
    ./src/info/rs_actioninfodist2.h \
    ./src/info/rs_actioninfodist.h \
    ./src/info/rs_actioninfoinside.h \
    ./src/info/rs_actioninfototallength.h \
    ./src/layers/rs_actionlayersadd.h \
    ./src/layers/rs_actionlayersduplicate.h \
    ./src/layers/rs_actionlayersedit.h \
    ./src/layers/rs_actionlayershideall.h \
    ./src/layers/rs_actionlayersremove.h \
    ./src/layers/rs_actionlayersshowall.h \
    ./src/layers/rs_actionlayerstogglelock.h \
    ./src/layers/rs_actionlayerstoggleview.h \
    ./src/misc/rs_actionback.h \
    ./src/misc/rs_actionbase.h \
    ./src/misc/rs_actiondefault.h \
    ./src/misc/rs_actionforward.h \
    ./src/misc/rs_actionlibraryinsert.h \
    ./src/misc/rs_actionparisdebugcreatecontainer.h \
    ./src/misc/rs_actionreset.h \
    ./src/modify/rs_actionmodifyattributes.h \
    ./src/modify/rs_actionmodifybevel.h \
    ./src/modify/rs_actionmodifydeletefree.h \
    ./src/modify/rs_actionmodifydelete.h \
    ./src/modify/rs_actionmodifydeletequick.h \
    ./src/modify/rs_actionmodifydivide2.h \
    ./src/modify/rs_actionmodifydivide.h \
    ./src/modify/rs_actionmodifyentity.h \
    ./src/modify/rs_actionmodifytext.h \
    ./src/modify/rs_actionmodifyexplode.h \
    ./src/modify/rs_actionmodifyexplodetext.h \
    ./src/modify/rs_actionmodifyisoproject.h \
    ./src/modify/rs_actionmodifymirror.h \
    ./src/modify/rs_actionmodifymove.h \
    ./src/modify/rs_actionmodifymoverotate.h \
    ./src/modify/rs_actionmodifyrotate2.h \
    ./src/modify/rs_actionmodifyrotate.h \
    ./src/modify/rs_actionmodifyround.h \
    ./src/modify/rs_actionmodifyscale.h \
    ./src/modify/rs_actionmodifystretch.h \
    ./src/modify/rs_actionmodifytoback.h \
    ./src/modify/rs_actionmodifytofront.h \
    ./src/modify/rs_actionmodifytrimamount.h \
    ./src/modify/rs_actionmodifytrim.h \
    ./src/modify/rs_actionmodifytrimboth.h \
    ./src/options/rs_actionoptionsdrawing.h \
    ./src/options/rs_actionoptionsgeneral.h \
    ./src/script/rs_actionscriptopenide.h \
    ./src/select/rs_actionselectall.h \
    ./src/select/rs_actiondeselectall.h \
    ./src/select/rs_actionselectbase.h \
    ./src/select/rs_actionselectcurrentlayer.h \
    ./src/select/rs_actionselectcontour.h \
    ./src/select/rs_actionselect.h \
    ./src/select/rs_actionselectintersected.h \
    ./src/select/rs_actiondeselectintersected.h \
    ./src/select/rs_actionselectinvert.h \
    ./src/select/rs_actionselectlayer.h \
    ./src/select/rs_actionselectsingle.h \
    ./src/select/rs_actionselectwindow.h \
    ./src/select/rs_actiondeselectwindow.h \
    ./src/snap/rs_actionlockrelativezero.h \
    ./src/snap/rs_actionsetrelativezero.h \
    ./src/snap/rs_actionsetsnapmode.h \
    ./src/snap/rs_actionsetsnapmodeauto.h \
    ./src/snap/rs_actionsetsnapmodefree.h \
    ./src/snap/rs_actionsetsnapmodegrid.h \
    ./src/snap/rs_actionsetsnapmodeendpoint.h \
    ./src/snap/rs_actionsetsnapmodeonentity.h \
    ./src/snap/rs_actionsetsnapmodecenter.h \
    ./src/snap/rs_actionsetsnapmodemiddle.h \
    ./src/snap/rs_actionsetsnapmodedist.h \
    ./src/snap/rs_actionsetsnapmodeintersection.h \
    ./src/snap/rs_actionsetsnapmodereference.h \
    ./src/snap/rs_actionsetsnapmodecoordinate.h \
    ./src/snap/rs_actionsetsnapmodecoordinatepolar.h \
    ./src/snap/rs_actionsetsnaprestriction.h \
    ./src/snap/rs_actionsetsnaprestrictionnothing.h \
    ./src/snap/rs_actionsetsnaprestrictionorthogonal.h \
    ./src/snap/rs_actionsetsnaprestrictionhorizontal.h \
    ./src/snap/rs_actionsetsnaprestrictionvertical.h \
    ./src/snap/rs_actionsnapintersectionmanual.h \
    ./src/view/rs_actionviewdraft.h \
    ./src/view/rs_actionviewgrid.h \
    ./src/view/rs_actionviewstatusbar.h \
    ./src/zoom/rs_actionzoomauto.h \
    ./src/zoom/rs_actionzoomautoy.h \
    ./src/zoom/rs_actionzoomin.h \
    ./src/zoom/rs_actionzoomout.h \
    ./src/zoom/rs_actionzoompan.h \
    ./src/zoom/rs_actionzoomprevious.h \
    ./src/zoom/rs_actionzoomredraw.h \
    ./src/zoom/rs_actionzoomselection.h \
    ./src/zoom/rs_actionzoomscroll.h \
    ./src/zoom/rs_actionzoomwindow.h

SOURCES = \
    ./src/blocks/rs_actionblocksadd.cpp \
    ./src/blocks/rs_actionblocksattributes.cpp \
    ./src/blocks/rs_actionblockscreate.cpp \
    ./src/blocks/rs_actionblocksedit.cpp \
    ./src/blocks/rs_actionblockseditinsert.cpp \
    ./src/blocks/rs_actionblocksinsert.cpp \
    ./src/blocks/rs_actionblocksremove.cpp \
    ./src/blocks/rs_actionblockstoggleview.cpp \
    ./src/blocks/rs_actionblocksshowall.cpp \
    ./src/blocks/rs_actionblockshideall.cpp \
    ./src/debugging/rs_actiondebugentities.cpp \
    ./src/debugging/rs_actiondebugguiactions.cpp \
    ./src/debugging/rs_actiondebugresize.cpp \
    ./src/dimension/rs_actiondimaligned.cpp \
    ./src/dimension/rs_actiondimangular.cpp \
    ./src/dimension/rs_actiondimdiametric.cpp \
    ./src/dimension/rs_actiondimension.cpp \
    ./src/dimension/rs_actiondimleader.cpp \
    ./src/dimension/rs_actiondimlinear.cpp \
    ./src/dimension/rs_actiondimradial.cpp \
    ./src/dimension/rs_actiondimordinate.cpp \
    ./src/dimension/rs_actiondimhorizontal.cpp \
    ./src/dimension/rs_actiondimvertical.cpp \
    ./src/dimension/rs_actiontoolregeneratedimensions.cpp \
    ./src/draw/rs_actiondrawarc3p.cpp \
    ./src/draw/rs_actiondrawarc.cpp \
    ./src/draw/rs_actiondrawarctangential.cpp \
    ./src/draw/rs_actiondrawcircle2p.cpp \
    ./src/draw/rs_actiondrawcircle3p.cpp \
    ./src/draw/rs_actiondrawcircle.cpp \
    ./src/draw/rs_actiondrawcirclecr.cpp \
    ./src/draw/rs_actiondrawellipsearcaxis.cpp \
    ./src/draw/rs_actiondrawellipseaxis.cpp \
    ./src/draw/rs_actiondrawhatch.cpp \
    ./src/draw/rs_actiondrawimage.cpp \
    ./src/draw/rs_actiondrawlineangle.cpp \
    ./src/draw/rs_actiondrawlinebisector.cpp \
    ./src/draw/rs_actiondrawline.cpp \
    ./src/draw/rs_actiondrawlinefree.cpp \
    ./src/snap/rs_actionsetsnapmodegrid.cpp \
    ./src/snap/rs_actionsetsnapmodeendpoint.cpp \
    ./src/snap/rs_actionsetsnapmodeonentity.cpp \
    ./src/snap/rs_actionsetsnapmodecenter.cpp \
    ./src/snap/rs_actionsetsnapmodemiddle.cpp \
    ./src/snap/rs_actionsetsnapmodedist.cpp \
    ./src/snap/rs_actionsetsnapmodeintersection.cpp \
    ./src/snap/rs_actionsetsnapmodereference.cpp \
    ./src/snap/rs_actionsetsnapmodecoordinate.cpp \
    ./src/snap/rs_actionsetsnapmodecoordinatepolar.cpp \
    ./src/draw/rs_actiondrawlinehorizontal.cpp \
    ./src/draw/rs_actiondrawlinehorvert.cpp \
    ./src/draw/rs_actiondrawlineparallel.cpp \
    ./src/draw/rs_actiondrawarcparallel.cpp \
    ./src/draw/rs_actiondrawcircleparallel.cpp \
    ./src/draw/rs_actiondrawlineparallelthrough.cpp \
    ./src/draw/rs_actiondrawlinepolygon2.cpp \
    ./src/draw/rs_actiondrawlinepolygon.cpp \
    ./src/draw/rs_actiondrawlinerectangle.cpp \
    ./src/draw/rs_actiondrawlinerelangle.cpp \
    ./src/draw/rs_actiondrawlinetangent1.cpp \
    ./src/draw/rs_actiondrawlinetangent2.cpp \
    ./src/draw/rs_actiondrawlinevertical.cpp \
    ./src/draw/rs_actiondrawlineorthogonal.cpp \
    ./src/draw/rs_actiondrawpoint.cpp \
    ./src/draw/rs_actiondrawspline.cpp \
    ./src/draw/rs_actiondrawtext.cpp \
    ./src/edit/rs_actioneditcopy.cpp \
    ./src/edit/rs_actioneditcopyautoref.cpp \
    ./src/edit/rs_actioneditcut.cpp \
    ./src/edit/rs_actioneditcutautoref.cpp \
    ./src/edit/rs_actioneditpaste.cpp \
    ./src/edit/rs_actioneditundo.cpp \
    ./src/edit/rs_actioneditredo.cpp \
    ./src/file/rs_actionfileclose.cpp \
    ./src/file/rs_actionfileexport.cpp \
    ./src/file/rs_actionfilenew.cpp \
    ./src/file/rs_actionfileopen.cpp \
    ./src/file/rs_actionfilepdf.cpp \
    ./src/file/rs_actionfilesvg.cpp \
    ./src/file/rs_actionfileprint.cpp \
    ./src/file/rs_actionfileprintpreview.cpp \
    ./src/file/rs_actionfilequit.cpp \
    ./src/file/rs_actionfilesaveas.cpp \
    ./src/file/rs_actionfilesave.cpp \
    ./src/info/rs_actioninfoangle.cpp \
    ./src/info/rs_actioninfoarea.cpp \
    ./src/info/rs_actioninfodist2.cpp \
    ./src/info/rs_actioninfodist.cpp \
    ./src/info/rs_actioninfoinside.cpp \
    ./src/info/rs_actioninfototallength.cpp \
    ./src/layers/rs_actionlayersadd.cpp \
    ./src/layers/rs_actionlayersduplicate.cpp \
    ./src/layers/rs_actionlayersedit.cpp \
    ./src/layers/rs_actionlayershideall.cpp \
    ./src/layers/rs_actionlayersremove.cpp \
    ./src/layers/rs_actionlayersshowall.cpp \
    ./src/layers/rs_actionlayerstogglelock.cpp \
    ./src/layers/rs_actionlayerstoggleview.cpp \
    ./src/misc/rs_actionback.cpp \
    ./src/misc/rs_actiondefault.cpp \
    ./src/misc/rs_actionforward.cpp \
    ./src/misc/rs_actionlibraryinsert.cpp \
    ./src/misc/rs_actionparisdebugcreatecontainer.cpp \
    ./src/misc/rs_actionreset.cpp \
    ./src/modify/rs_actionmodifyattributes.cpp \
    ./src/modify/rs_actionmodifybevel.cpp \
    ./src/modify/rs_actionmodifydelete.cpp \
    ./src/modify/rs_actionmodifydeletefree.cpp \
    ./src/modify/rs_actionmodifydeletequick.cpp \
    ./src/modify/rs_actionmodifydivide2.cpp \
    ./src/modify/rs_actionmodifydivide.cpp \
    ./src/modify/rs_actionmodifyentity.cpp \
    ./src/modify/rs_actionmodifytext.cpp \
    ./src/modify/rs_actionmodifyexplode.cpp \
    ./src/modify/rs_actionmodifyexplodetext.cpp \
    ./src/modify/rs_actionmodifyisoproject.cpp \
    ./src/modify/rs_actionmodifymirror.cpp \
    ./src/modify/rs_actionmodifymove.cpp \
    ./src/modify/rs_actionmodifymoverotate.cpp \
    ./src/modify/rs_actionmodifyrotate2.cpp \
    ./src/modify/rs_actionmodifyrotate.cpp \
    ./src/modify/rs_actionmodifyround.cpp \
    ./src/modify/rs_actionmodifyscale.cpp \
    ./src/modify/rs_actionmodifystretch.cpp \
    ./src/modify/rs_actionmodifytoback.cpp \
    ./src/modify/rs_actionmodifytofront.cpp \
    ./src/modify/rs_actionmodifytrimamount.cpp \
    ./src/modify/rs_actionmodifytrim.cpp \
    ./src/modify/rs_actionmodifytrimboth.cpp \
    ./src/options/rs_actionoptionsdrawing.cpp \
    ./src/options/rs_actionoptionsgeneral.cpp \
    ./src/script/rs_actionscriptopenide.cpp \
    ./src/select/rs_actionselectall.cpp \
    ./src/select/rs_actiondeselectall.cpp \
    ./src/select/rs_actionselectbase.cpp \
    ./src/select/rs_actionselectcurrentlayer.cpp \
    ./src/select/rs_actionselectcontour.cpp \
    ./src/select/rs_actionselect.cpp \
    ./src/select/rs_actionselectintersected.cpp \
    ./src/select/rs_actiondeselectintersected.cpp \
    ./src/select/rs_actionselectinvert.cpp \
    ./src/select/rs_actionselectlayer.cpp \
    ./src/select/rs_actionselectsingle.cpp \
    ./src/select/rs_actionselectwindow.cpp \
    ./src/select/rs_actiondeselectwindow.cpp \
    ./src/snap/rs_actionlockrelativezero.cpp \
    ./src/snap/rs_actionsetrelativezero.cpp \
    ./src/snap/rs_actionsetsnapmode.cpp \
    ./src/snap/rs_actionsetsnapmodeauto.cpp \
    ./src/snap/rs_actionsetsnapmodefree.cpp \
    ./src/snap/rs_actionsetsnaprestriction.cpp \
    ./src/snap/rs_actionsetsnaprestrictionnothing.cpp \
    ./src/snap/rs_actionsetsnaprestrictionorthogonal.cpp \
    ./src/snap/rs_actionsetsnaprestrictionhorizontal.cpp \
    ./src/snap/rs_actionsetsnaprestrictionvertical.cpp \
    ./src/snap/rs_actionsnapintersectionmanual.cpp \
    ./src/view/rs_actionviewdraft.cpp \
    ./src/view/rs_actionviewgrid.cpp \
    ./src/view/rs_actionviewstatusbar.cpp \
    ./src/zoom/rs_actionzoomauto.cpp \
    ./src/zoom/rs_actionzoomautoy.cpp \
    ./src/zoom/rs_actionzoomin.cpp \
    ./src/zoom/rs_actionzoomout.cpp \
    ./src/zoom/rs_actionzoompan.cpp \
    ./src/zoom/rs_actionzoomprevious.cpp \
    ./src/zoom/rs_actionzoomredraw.cpp \
    ./src/zoom/rs_actionzoomselection.cpp \
    ./src/zoom/rs_actionzoomscroll.cpp \
    ./src/zoom/rs_actionzoomwindow.cpp

HEADERS += \
    ./src/draw/rs_actiondrawpolyline.h \
    ./src/draw/rs_actionpolylineadd.h \
    ./src/draw/rs_actionpolylineappend.h \
    ./src/draw/rs_actionpolylinedel.h \
    ./src/draw/rs_actionpolylinedelbetween.h \
    ./src/draw/rs_actionpolylineequidistant.h \
    ./src/draw/rs_actionpolylinefromsegments.h \
    ./src/draw/rs_actionpolylinetrim.h
SOURCES += \
    ./src/draw/rs_actiondrawpolyline.cpp \
    ./src/draw/rs_actionpolylineadd.cpp \
    ./src/draw/rs_actionpolylineappend.cpp \
    ./src/draw/rs_actionpolylinedel.cpp \
    ./src/draw/rs_actionpolylinedelbetween.cpp \
    ./src/draw/rs_actionpolylineequidistant.cpp \
    ./src/draw/rs_actionpolylinefromsegments.cpp \
    ./src/draw/rs_actionpolylinetrim.cpp

rs_algo {
    HEADERS += \
        ./src/modify/rs_actionmodifytriangulate.h \
        ./src/modify/rs_actionmodifydobooleanoperation.h
    SOURCES += \
        ./src/modify/rs_actionmodifytriangulate.cpp \
        ./src/modify/rs_actionmodifydobooleanoperation.cpp
}

TRANSLATIONS = \
    ./src/ts/qcadactions_bg.ts \
    ./src/ts/qcadactions_cs.ts \
    ./src/ts/qcadactions_cy.ts \
    ./src/ts/qcadactions_en.ts \
    ./src/ts/qcadactions_et.ts \
    ./src/ts/qcadactions_da.ts \
    ./src/ts/qcadactions_de.ts \
    ./src/ts/qcadactions_el.ts \
    ./src/ts/qcadactions_es.ts \
    ./src/ts/qcadactions_fr.ts \
    ./src/ts/qcadactions_hu.ts \
    ./src/ts/qcadactions_id.ts \
    ./src/ts/qcadactions_it.ts \
    ./src/ts/qcadactions_lt.ts \
    ./src/ts/qcadactions_ja.ts \
    ./src/ts/qcadactions_nl.ts \
    ./src/ts/qcadactions_no.ts \
    ./src/ts/qcadactions_pa.ts \
    ./src/ts/qcadactions_pl.ts \
    ./src/ts/qcadactions_pt.ts \
    ./src/ts/qcadactions_ru.ts \
    ./src/ts/qcadactions_sk.ts \
    ./src/ts/qcadactions_sv.ts \
    ./src/ts/qcadactions_tr.ts \
    ./src/ts/qcadactions_zh.ts

RESOURCES   = ./src/qcadactions.qrc

TARGET      = qcadactions

OBJECTS_DIR = .obj
MOC_DIR     = .moc
RCC_DIR     = .res

